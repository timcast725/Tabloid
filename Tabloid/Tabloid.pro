TEMPLATE = app
TARGET = audiorecorder

QT += multimedia

win32:INCLUDEPATH += $$PWD

HEADERS = \
    mainwidget.h \
    engine.h \
    wavfile.h \
    utils.h \
    progressbar.h \
    levelmeter.h \
    settingsdialog.h

SOURCES = \
    main.cpp \
    engine.cpp \
    mainwidget.cpp \
    wavfile.cpp \
    utils.cpp \
    progressbar.cpp \
    levelmeter.cpp \
    settingsdialog.cpp

maemo*: {
    FORMS += audiorecorder_small.ui
}else {
    FORMS +=
}

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
INSTALLS += target

QT+=widgets

FORMS +=
