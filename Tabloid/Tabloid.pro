TEMPLATE = app
TARGET = audiorecorder

QT += multimedia

win32:INCLUDEPATH += $$PWD

HEADERS = \
    mainwidget.h \
    engine.h

SOURCES = \
    main.cpp \
    engine.cpp \
    mainwidget.cpp

maemo*: {
    FORMS += audiorecorder_small.ui
}else {
    FORMS +=
}

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
INSTALLS += target

QT+=widgets

FORMS +=
