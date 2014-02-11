// Copyright 2013 Zev Battad

// This file is part of Tabloid.

// Tabloid is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Tabloid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Tabloid.  If not, see <http://www.gnu.org/licenses/>.

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
