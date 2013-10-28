#-------------------------------------------------
#
# Project created by QtCreator 2013-10-19T16:48:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT       += multimedia

TARGET = FileHandler
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    file_handler.cpp \
    wavfile.cpp \
    utils.cpp

HEADERS += \
    file_handler.h \
    wavfile.h \
    utils.h
