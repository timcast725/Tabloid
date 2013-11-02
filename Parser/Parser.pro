#-------------------------------------------------
#
# Project created by QtCreator 2013-11-01T22:59:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Parser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    parser.cpp

HEADERS += \
    parser.h

unix:!macx: LIBS += -laubio

unix:!macx: LIBS += -laubioext
