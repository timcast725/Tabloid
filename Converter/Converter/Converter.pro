#-------------------------------------------------
#
# Project created by QtCreator 2013-11-06T16:20:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Converter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../../Tabloid/sheet_music.cpp \
    ../../Tabloid/note.cpp \
    ../../Tabloid/measure.cpp \
    ../../third-party/midifile/src/MidiFile.cpp

HEADERS += \
    converter.h \
    ../../Tabloid/sheet_music.h \
    ../../Tabloid/note.h \
    ../../Tabloid/measure.h \
    ../../third-party/midifile/include/MidiFile.h
