#ifndef MIDICONVERTER_H
#define MIDICONVERTER_H

#include "MidiFile.h"

#include "converter.h"
#include "sheet_music.h"

using ::std::string;

class MidiConverter : public Converter
{
private:
    string output_file_name_;
public:
    MidiConverter();
    MidiConverter(string outfile_name);
    bool Convert(SheetMusic sheet);
    void SetFileNameToDefault();
    void set_output_file_name(string new_name);
    string output_file_name();
};

#endif // MIDICONVERTER_H
