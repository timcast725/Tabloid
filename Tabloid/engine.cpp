#include "engine.h"

#include <math.h>

#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QMetaObject>
#include <QSet>
#include <QThread>

Engine::Engine(QObject *parent)
    :   QObject(parent)
    ,   m_mode(QAudio::AudioInput)
    ,   m_state(QAudio::StoppedState)
    ,   m_generateTone(false)
    ,   m_file(0)
    ,   m_analysisFile(0)
    ,   m_availableAudioInputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    ,   m_audioInputDevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(0)
    ,   m_audioInputIODevice(0)
    ,   m_recordPosition(0)
    ,   m_availableAudioOutputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_playPosition(0)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_dataLength(0)
    ,   m_levelBufferLength(0)
    ,   m_rmsLevel(0.0)
    ,   m_peakLevel(0.0)
    ,   m_spectrumBufferLength(0)
    ,   m_spectrumAnalyser()
    ,   m_spectrumPosition(0)
    ,   m_count(0)
{
    initialize();

#ifdef DUMP_DATA
    createOutputDir();
#endif

}

Engine::~Engine()
{

}

bool Engine::loadFile(const QString &fileName)
{
    reset();
    bool result = false;
    Q_ASSERT(!m_file);
    Q_ASSERT(!fileName.isEmpty());
    m_file = new WavFile(this);
    if (m_file->open(fileName)) {
        if (isPCMS16LE(m_file->fileFormat())) {
            result = initialize();
        } else {
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_file->fileFormat()));
        }
    } else {
        emit errorMessage(tr("Could not open file"), fileName);
    }
    if (result) {
        m_analysisFile = new WavFile(this);
        m_analysisFile->open(fileName);
    }
    return result;
}

bool Engine::initializeRecord()
{
    reset();
    ENGINE_DEBUG << "Engine::initializeRecord";
    Q_ASSERT(!m_generateTone);
    Q_ASSERT(!m_file);
    m_generateTone = false;
    m_tone = SweptTone();
    return initialize();
}

qint64 Engine::bufferLength() const
{
    return m_file ? m_file->size() : m_bufferLength;
}

void Engine::setWindowFunction(WindowFunction type)
{
    m_spectrumAnalyser.setWindowFunction(type);
}

//Public Slots

void Engine::startRecording()
{
    if (m_audioInput) {
        if (QAudio::AudioInput == m_mode &&
            QAudio::SuspendedState == m_state) {
            m_audioInput->resume();
        } else {
            m_spectrumAnalyser.cancelCalculation();
            spectrumChanged(0, 0, FrequencySpectrum());

            m_buffer.fill(0);
            setRecordPosition(0, true);
            stopPlayback();
            m_mode = QAudio::AudioInput;
            CHECKED_CONNECT(m_audioInput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(audioStateChanged(QAudio::State)));
            CHECKED_CONNECT(m_audioInput, SIGNAL(notify()),
                            this, SLOT(audioNotify()));
            m_count = 0;
            m_dataLength = 0;
            emit dataLengthChanged(0);
            m_audioInputIODevice = m_audioInput->start();
            CHECKED_CONNECT(m_audioInputIODevice, SIGNAL(readyRead()),
                            this, SLOT(audioDataReady()));
        }
    }
}

void Engine::startPlayback()
{
    if (m_audioOutput) {
        if (QAudio::AudioOutput == m_mode &&
            QAudio::SuspendedState == m_state) {
#ifdef Q_OS_WIN
            // The Windows backend seems to internally go back into ActiveState
            // while still returning SuspendedState, so to ensure that it doesn't
            // ignore the resume() call, we first re-suspend
            m_audioOutput->suspend();
#endif
            m_audioOutput->resume();
        } else {
            m_spectrumAnalyser.cancelCalculation();
            spectrumChanged(0, 0, FrequencySpectrum());
            setPlayPosition(0, true);
            stopRecording();
            m_mode = QAudio::AudioOutput;
            CHECKED_CONNECT(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(audioStateChanged(QAudio::State)));
            CHECKED_CONNECT(m_audioOutput, SIGNAL(notify()),
                            this, SLOT(audioNotify()));
            m_count = 0;
            if (m_file) {
                m_file->seek(0);
                m_bufferPosition = 0;
                m_dataLength = 0;
                m_audioOutput->start(m_file);
            } else {
                m_audioOutputIODevice.close();
                m_audioOutputIODevice.setBuffer(&m_buffer);
                m_audioOutputIODevice.open(QIODevice::ReadOnly);
                m_audioOutput->start(&m_audioOutputIODevice);
            }
        }
    }
}

void Engine::suspend()
{
    if (QAudio::ActiveState == m_state ||
        QAudio::IdleState == m_state) {
        switch (m_mode) {
        case QAudio::AudioInput:
            m_audioInput->suspend();
            break;
        case QAudio::AudioOutput:
            m_audioOutput->suspend();
            break;
        }
    }
}

void Engine::setAudioInputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioInputDevice.deviceName()) {
        m_audioInputDevice = device;
        initialize();
    }
}

void Engine::setAudioOutputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioOutputDevice.deviceName()) {
        m_audioOutputDevice = device;
        initialize();
    }
}

//Private Slots

void Engine::audioNotify()
{
    switch (m_mode) {
    case QAudio::AudioInput: {
            const qint64 recordPosition = qMin(m_bufferLength, audioLength(m_format, m_audioInput->processedUSecs()));
            setRecordPosition(recordPosition);
            const qint64 levelPosition = m_dataLength - m_levelBufferLength;
            if (levelPosition >= 0)
                calculateLevel(levelPosition, m_levelBufferLength);
            if (m_dataLength >= m_spectrumBufferLength) {
                const qint64 spectrumPosition = m_dataLength - m_spectrumBufferLength;
                calculateSpectrum(spectrumPosition);
            }
            emit bufferChanged(0, m_dataLength, m_buffer);
        }
        break;
    case QAudio::AudioOutput: {
            const qint64 playPosition = audioLength(m_format, m_audioOutput->processedUSecs());
            setPlayPosition(qMin(bufferLength(), playPosition));
            const qint64 levelPosition = playPosition - m_levelBufferLength;
            const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
            if (m_file) {
                if (levelPosition > m_bufferPosition ||
                    spectrumPosition > m_bufferPosition ||
                    qMax(m_levelBufferLength, m_spectrumBufferLength) > m_dataLength) {
                    m_bufferPosition = 0;
                    m_dataLength = 0;
                    // Data needs to be read into m_buffer in order to be analysed
                    const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
                    const qint64 readEnd = qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
                    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
                    qDebug() << "Engine::audioNotify [1]"
                             << "analysisFileSize" << m_analysisFile->size()
                             << "readPos" << readPos
                             << "readLen" << readLen;
                    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
                        qDebug() << "Engine::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
                    } else {
                        qDebug() << "Engine::audioNotify [2]" << "file seek error";
                    }
                    emit bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                }
            } else {
                if (playPosition >= m_dataLength)
                    stopPlayback();
            }
            if (levelPosition >= 0 && levelPosition + m_levelBufferLength < m_bufferPosition + m_dataLength)
                calculateLevel(levelPosition, m_levelBufferLength);
            if (spectrumPosition >= 0 && spectrumPosition + m_spectrumBufferLength < m_bufferPosition + m_dataLength)
                calculateSpectrum(spectrumPosition);
        }
        break;
    }
}

void Engine::audioStateChanged(QAudio::State state)
{
    ENGINE_DEBUG << "Engine::audioStateChanged from" << m_state
                 << "to" << state;

    if (QAudio::IdleState == state && m_file && m_file->pos() == m_file->size()) {
        stopPlayback();
    } else {
        if (QAudio::StoppedState == state) {
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (m_mode) {
            case QAudio::AudioInput:
                error = m_audioInput->error();
                break;
            case QAudio::AudioOutput:
                error = m_audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) {
                reset();
                return;
            }
        }
        setState(state);
    }
}

void Engine::audioDataReady()
{
    Q_ASSERT(0 == m_bufferPosition);
    const qint64 bytesReady = m_audioInput->bytesReady();
    const qint64 bytesSpace = m_buffer.size() - m_dataLength;
    const qint64 bytesToRead = qMin(bytesReady, bytesSpace);

    const qint64 bytesRead = m_audioInputIODevice->read(
                                       m_buffer.data() + m_dataLength,
                                       bytesToRead);

    if (bytesRead) {
        m_dataLength += bytesRead;
        emit dataLengthChanged(dataLength());
    }

    if (m_buffer.size() == m_dataLength)
        stopRecording();
}


