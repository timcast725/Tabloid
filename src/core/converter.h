// Copyright 2014 Gary Lu

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

#ifndef CONVERTER_H
#define CONVERTER_H

#include "sheet_music.h"

#include <string>
#include <fstream>
#include <vector>

class Converter
{
public:
    Converter() { }
    // Convert our sheet music data structure. into MusicXML.
    // name is the name of the file the converter outputs to.
    // sheet is our sheet music to convert from. Get this from the parser.
    virtual bool convert(const std::string &name, const SheetMusic &sheet) = 0;
protected:
    virtual void getNote(int pitch, std::string &letter, std::string &alter);
    void open(std::string tag, std::string option="");
    void print(std::string tag, std::string content);
    void close();
    std::ofstream output;
    std::vector<std::string> tags;
    int measure_number;
    int last_key;
    int last_beats;
    int last_beat_type;
    bool last_clef;
};


#endif
// CONVERTER_H
