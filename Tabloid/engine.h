#ifndef ENGINE_H
#define ENGINE_H

#include <wavfile.h>
#include <utils.h>
#include <parser.h>
#include <sheet_music.h>

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QObject>
#include <QVector>


/**
 * This class interfaces with the Qt Multimedia audio classes, and also with
 * the Parser class.  Its role is to manage the capture and playback
 * of audio data, meanwhile performing real-time analysis of the audio level
 * and audio analyzer. This class is analagous to the message handler.
 */

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioOutput;
QT_END_NAMESPACE

class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject *parent = 0);
    ~Engine();

    const QList<QAudioDeviceInfo> &availableAudioInputDevices() const
                                    { return m_availableAudioInputDevices; }

    const QList<QAudioDeviceInfo> &availableAudioOutputDevices() const
                                    { return m_availableAudioOutputDevices; }

    QAudio::Mode mode() const { return m_mode; }
    QAudio::State state() const { return m_state; }

    /**
     * \return Current audio format
     * \note May be QAudioFormat() if engine is not initialized
     */
    const QAudioFormat& format() const { return m_format; }

    void reset();

    bool loadFile(const QString &fileName);

    bool initializeRecord();

    /**
     * Position of the audio input device.
     * \return Position in bytes.
     */
    qint64 recordPosition() const { return m_recordPosition; }

    /**
     * RMS level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal rmsLevel() const { return m_rmsLevel; }

    /**
     * Peak level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal peakLevel() const { return m_peakLevel; }

    /**
     * Position of the audio output device.
     * \return Position in bytes.
     */
    qint64 playPosition() const { return m_playPosition; }

    /**
     * Length of the internal engine buffer.
     * \return Buffer length in bytes.
     */
    qint64 bufferLength() const;

    /**
     * Amount of data held in the buffer.
     * \return Data length in bytes.
     */
    qint64 dataLength() const { return m_dataLength; }

public slots:
    void startRecording();
    void startPlayback();
    void suspend();
    void setAudioInputDevice(const QAudioDeviceInfo &device);
    void setAudioOutputDevice(const QAudioDeviceInfo &device);

signals:
    void stateChanged(QAudio::Mode mode, QAudio::State state);

    /**
     * Informational message for non-modal display
     */
    void infoMessage(const QString &message, int durationMs);

    /**
     * Error message for modal display
     */
    void errorMessage(const QString &heading, const QString &detail);

    /**
     * Format of audio data has changed
     */
    void formatChanged(const QAudioFormat &format);

    /**
     * Length of buffer has changed.
     * \param duration Duration in microseconds
     */
    void bufferLengthChanged(qint64 duration);

    /**
     * Amount of data in buffer has changed.
     * \param Length of data in bytes
     */
    void dataLengthChanged(qint64 duration);

    /**
     * Position of the audio input device has changed.
     * \param position Position in bytes
     */
    void recordPositionChanged(qint64 position);

    /**
     * Position of the audio output device has changed.
     * \param position Position in bytes
     */
    void playPositionChanged(qint64 position);

    /**
     * Level changed
     * \param rmsLevel RMS level in range 0.0 - 1.0
     * \param peakLevel Peak level in range 0.0 - 1.0
     * \param numSamples Number of audio samples analyzed
     */
    void levelChanged(qreal rmsLevel, qreal peakLevel, int numSamples);

    /**
     * Buffer containing audio data has changed.
     * \param position Position of start of buffer in bytes
     * \param buffer   Buffer
     */
    void bufferChanged(qint64 position, qint64 length, const QByteArray &buffer);

private slots:
    void audioNotify();
    void audioStateChanged(QAudio::State state);
    void audioDataReady();

private:
    void resetAudioDevices();
    bool initialize();
    bool selectFormat();
    void stopRecording();
    void stopPlayback();
    void setState(QAudio::State state);
    void setState(QAudio::Mode mode, QAudio::State state);
    void setFormat(const QAudioFormat &format);
    void setRecordPosition(qint64 position, bool forceEmit = false);
    void setPlayPosition(qint64 position, bool forceEmit = false);
    void calculateLevel(qint64 position, qint64 length);
    void parse(qint64 position);
    void setLevel(qreal rmsLevel, qreal peakLevel, int numSamples);
    void parse();

private:
    QAudio::Mode        m_mode;
    QAudio::State       m_state;

    WavFile*            m_file;
    // We need a second file handle via which to read data into m_buffer
    // for analysis
    WavFile*            m_analysisFile;

    QAudioFormat        m_format;

    const QList<QAudioDeviceInfo> m_availableAudioInputDevices;
    QAudioDeviceInfo    m_audioInputDevice;
    QAudioInput*        m_audioInput;
    QIODevice*          m_audioInputIODevice;
    qint64              m_recordPosition;

    const QList<QAudioDeviceInfo> m_availableAudioOutputDevices;
    QAudioDeviceInfo    m_audioOutputDevice;
    QAudioOutput*       m_audioOutput;
    qint64              m_playPosition;
    QBuffer             m_audioOutputIODevice;

    QByteArray          m_buffer;
    qint64              m_bufferPosition;
    qint64              m_bufferLength;
    qint64              m_dataLength;

    int                 m_levelBufferLength;
    qreal               m_rmsLevel;
    qreal               m_peakLevel;

    Parser              m_parser;
    SheetMusic          m_sheetmusic;

    int                 m_count;

};

#endif // ENGINE_H
