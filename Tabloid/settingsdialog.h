#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAudioDeviceInfo>
#include <utils.h>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QSlider;
class QSpinBox;
class QGridLayout;
QT_END_NAMESPACE

/**
 * Dialog used to control settings such as the audio input / output device
 * and the windowing function.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(const QList<QAudioDeviceInfo> &availableInputDevices,
                   const QList<QAudioDeviceInfo> &availableOutputDevices,
                   QWidget *parent = 0);
    ~SettingsDialog();

    const QAudioDeviceInfo &inputDevice() const { return m_inputDevice; }
    const QAudioDeviceInfo &outputDevice() const { return m_outputDevice; }

private slots:
    void inputDeviceChanged(int index);
    void outputDeviceChanged(int index);

private:
    QAudioDeviceInfo m_inputDevice;
    QAudioDeviceInfo m_outputDevice;

    QComboBox *m_inputDeviceComboBox;
    QComboBox *m_outputDeviceComboBox;
};

#endif // SETTINGSDIALOG_H
