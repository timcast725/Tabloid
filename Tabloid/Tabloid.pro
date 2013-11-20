TEMPLATE = app
TARGET = audiorecorder

QT += multimedia

win32:INCLUDEPATH += $$PWD

INCLUDEPATH += ../third-party/midifile/include \
    ../Converter/Converter

HEADERS = \
    mainwidget.h \
    engine.h \
    wavfile.h \
    utils.h \
    progressbar.h \
    levelmeter.h \
    settingsdialog.h \
    note.h \
    sheet_music.h \
    measure.h \
    parser.h \
    ../third-party/midifile/include/MidiFile.h \
    ../third-party/midifile/include/Array.h \
    ../third-party/midifile/include/sigConfiguration.h \
    ../third-party/midifile/include/SigCollection.h \
    ../third-party/midifile/include/Options.h \
    ../third-party/midifile/include/Options_private.h \
    ../third-party/midifile/include/FileIO.h \
    ../third-party/midifile/src/MidiFile.cpp.autosave \
    ../Converter/Converter/converter.h \
    ../Converter/Converter/midi_converter.h

SOURCES = \
    main.cpp \
    engine.cpp \
    mainwidget.cpp \
    wavfile.cpp \
    utils.cpp \
    progressbar.cpp \
    levelmeter.cpp \
    settingsdialog.cpp \
    note.cpp \
    sheet_music.cpp \
    measure.cpp \
    parser.cpp \
    ../third-party/midifile/include/SigCollection.cpp \
    ../third-party/midifile/src/Options.cpp \
    ../third-party/midifile/src/Options_private.cpp \
    ../third-party/midifile/src/MidiFile.cpp \
    ../third-party/midifile/src/FileIO.cpp \
    ../Converter/Converter/converter.cpp \
    ../Converter/Converter/midi_converter.cpp

maemo*: {
    FORMS += audiorecorder_small.ui
}else {
    FORMS +=
}

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
INSTALLS += target

QT+=widgets

QMAKE_CXXFLAGS += -std=gnu++0x

unix:!macx: LIBS += -laubio

unix:!macx: LIBS += -laubioext

FORMS +=
