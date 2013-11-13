#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QAudio>
#include <QIcon>
#include <QWidget>

/*Put all classes in here
 *
 *class Engine
 *class Parser
 *class Converter
 *class SheetMusic
 *class ...
 *
 */

class Engine;
class LevelMeter;
class ProgressBar;
class SettingsDialog;

QT_BEGIN_NAMESPACE
class QAction;
class QAudioFormat;
class QLabel;
class QMenu;
class QPushButton;
QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    // QObject
    void timerEvent(QTimerEvent *event);

public slots:
    void stateChanged(QAudio::Mode mode, QAudio::State state);
    void formatChanged(const QAudioFormat &format);
    void infoMessage(const QString &message, int timeoutMs);
    void errorMessage(const QString &heading, const QString &detail);
    void audioPositionChanged(qint64 position);
    void bufferLengthChanged(qint64 length);

private slots:
    void showFileDialog();
    void showSettingsDialog();
    void initializeRecord();
    void updateModeMenu();
    void updateButtonStates();

private:
    void createUi();
    void createMenus();
    void connectUi();
    void reset();

    enum Mode {
        NoMode,
        RecordMode,
        LoadFileMode
    };

    void setMode(Mode mode);

private:
    Mode                    m_mode;

    Engine*                 m_engine;

    ProgressBar*            m_progressBar;
    LevelMeter*             m_levelMeter;

    QPushButton*            m_modeButton;
    QPushButton*            m_recordButton;
    QIcon                   m_recordIcon;
    QPushButton*            m_pauseButton;
    QIcon                   m_pauseIcon;
    QPushButton*            m_playButton;
    QPushButton*            m_parseButton;
    QIcon                   m_playIcon;
    QPushButton*            m_settingsButton;
    QIcon                   m_settingsIcon;

    QLabel*                 m_infoMessage;
    int                     m_infoMessageTimerId;

    SettingsDialog*         m_settingsDialog;

    QMenu*                  m_modeMenu;
    QAction*                m_loadFileAction;
    QAction*                m_recordAction;

};







#endif // MAINWIDGET_H
