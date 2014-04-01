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
//

#ifndef SHEET_MUSIC_CONVERTER_H
#define SHEET_MUSIC_CONVERTER_H

#include "converter.h"
#include "sheet_music.h"

#include <fstream>
#include <string>
#include <vector>

class SheetMusicConverter : public Converter
{
public:
    SheetMusicConverter();
    bool convert(const std::string &name, const SheetMusic &sheet);

private:
    int measure_number;
    int last_key;
    int last_beats;
    int last_beat_type;
    bool last_clef;
    std::ofstream output;
    std::vector<std::string> tags;
    void open(std::string tag, std::string option="");
    void print(std::string tag, std::string content);
    void close();
    void addNote(int pitch, float start, float duration, float beat_duration, int divisions=1);
    void addMeasure(int divisions=1, int key=0, int beats=4, int beat_type=4, bool treble=true);
};


#endif
// CONVERTER_H
