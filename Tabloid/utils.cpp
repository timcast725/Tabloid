#include <QAudioFormat>
#include "utils.h"

qint64 audioDuration(const QAudioFormat &format, qint64 bytes)
{
    return (bytes * 1000000) /
        (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8));
}

qint64 audioLength(const QAudioFormat &format, qint64 microSeconds)
{
   qint64 result = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
       * microSeconds / 1000000;
   result -= result % (format.channelCount() * format.sampleSize());
   return result;
}

qreal nyquistFrequency(const QAudioFormat &format)
{
    return format.sampleRate() / 2;
}

QString formatToString(const QAudioFormat &format)
{
    QString result;

    if (QAudioFormat() != format) {
        if (format.codec() == "audio/pcm") {
            Q_ASSERT(format.sampleType() == QAudioFormat::SignedInt);

            const QString formatEndian = (format.byteOrder() == QAudioFormat::LittleEndian)
                ?   QString("LE") : QString("BE");

            QString formatType;
            switch (format.sampleType()) {
            case QAudioFormat::SignedInt:
                formatType = "signed";
                break;
            case QAudioFormat::UnSignedInt:
                formatType = "unsigned";
                break;
            case QAudioFormat::Float:
                formatType = "float";
                break;
            case QAudioFormat::Unknown:
                formatType = "unknown";
                break;
            }

            QString formatChannels = QString("%1 channels").arg(format.channelCount());
            switch (format.channelCount()) {
            case 1:
                formatChannels = "mono";
                break;
            case 2:
                formatChannels = "stereo";
                break;
            }

            result = QString("%1 Hz %2 bit %3 %4 %5")
                .arg(format.sampleRate())
                .arg(format.sampleSize())
                .arg(formatType)
                .arg(formatEndian)
                .arg(formatChannels);
        } else {
            result = format.codec();
        }
    }

    return result;
}

bool isPCM(const QAudioFormat &format)
{
    return (format.codec() == "audio/pcm");
}


bool isPCMS16LE(const QAudioFormat &format)
{
    return isPCM(format) &&
           format.sampleType() == QAudioFormat::SignedInt &&
           format.sampleSize() == 16 &&
           format.byteOrder() == QAudioFormat::LittleEndian;
}

const qint16  PCMS16MaxValue     =  32767;
const quint16 PCMS16MaxAmplitude =  32768; // because minimum is -32768

qreal pcmToReal(qint16 pcm)
{
    return qreal(pcm) / PCMS16MaxAmplitude;
}

qint16 realToPcm(qreal real)
{
    return real * PCMS16MaxValue;
}

// writes the recorded audio data into a .WAV file
void writeWaveFile(QString filename, QByteArray data, qint64 length, QAudioFormat format)
{
    if (length == 0)
    {
        printf("Could not save recorded data, nothing recorded\n");
        return;
    }
    std::string filename_str  = filename.toStdString();


    FILE* file = fopen(filename_str.c_str() , "wb");

    if (file)
    {
        // write wave header
        unsigned short formatType =	1;
        unsigned short numChannels = format.channelCount();
        unsigned long  sampleRate = format.sampleRate();
        unsigned short bitsPerChannel = format.sampleSize() * 8;
        unsigned short bytesPerSample = format.bytesPerFrame();
        unsigned long  bytesPerSecond = format.bytesForDuration(1000000);
        unsigned long  dataLen = length;

        const int fmtChunkLen = 16;
        const int waveHeaderLen = 4 + 8 + fmtChunkLen + 8;

        unsigned long totalLen = waveHeaderLen + dataLen;

        fwrite("RIFF", 4, 1, file);
        fwrite(&totalLen, 4, 1, file);
        fwrite("WAVE", 4, 1, file);
        fwrite("fmt ", 4, 1, file);
        fwrite(&fmtChunkLen, 4, 1, file);
        fwrite(&formatType, 2, 1, file);
        fwrite(&numChannels, 2, 1, file);
        fwrite(&sampleRate, 4, 1, file);
        fwrite(&bytesPerSecond, 4, 1, file);
        fwrite(&bytesPerSample, 2, 1, file);
        fwrite(&bitsPerChannel, 2, 1, file);

        // write data

        fwrite("data", 4, 1, file);
        fwrite(&dataLen, 4, 1, file);
        fwrite(data, dataLen, 1, file);

        // finish

        printf("Saved audio as %s\n", filename_str.c_str());
        fclose(file);
    }
    else
        printf("Could not open %s to write audio data\n", filename_str.c_str());

}
