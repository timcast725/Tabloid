#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T13:15:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Converter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../third-party/midifile/include \
    ../../tabloid

SOURCES += main.cpp \
    ../../third-party/midifile/include/SigCollection.cpp \
    ../../third-party/midifile/include/Array.cpp \
    ../../third-party/midifile/src/Options_private.cpp \
    ../../third-party/midifile/src/Options.cpp \
    ../../third-party/midifile/src/MidiFile.cpp \
    ../../third-party/midifile/src/FileIO.cpp \
    ../../tabloid/sheet_music.cpp \
    ../../tabloid/measure.cpp \
    converter.cpp \
    ../../tabloid/note.cpp

HEADERS += \
    ../../third-party/midifile/include/sigConfiguration.h \
    ../../third-party/midifile/include/SigCollection.h \
    ../../third-party/midifile/include/Options_private.h \
    ../../third-party/midifile/include/Options.h \
    ../../third-party/midifile/include/MidiFile.h \
    ../../third-party/midifile/include/FileIO.h \
    ../../third-party/midifile/include/Array.h \
    converter.h \
    ../../tabloid/sheet_music.h \
    ../../tabloid/measure.h \
    ../../tabloid/note.h
