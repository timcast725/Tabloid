// Copyright 2014 Timothy Castiglia

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

//Standard tunings of E, Eb, D, C#, C, B and Bb are supported
//These tunings are named by the first string's tuning
//7, 8 and 9 string guitars supported (?)

#ifndef GUITAR_TAB_CONVERTER_H
#define GUITAR_TAB_CONVERTER_H

#include "converter.h"
#include "sheet_music.h"

#include <fstream>
#include <string>
#include <vector>

class GuitarTabConverter : public Converter
{
    public:
        GuitarTabConverter();
        GuitarTabConverter(std::vector<std::pair<std::string,std::string> >& input);
        bool convert(const std::string &name, const SheetMusic &sheet);
    private:
        void getNote(int pitch, std::string &letter, std::string &alter,
                     std::string &stringNum, std::string& fret);
        void addNote(int pitch, float start, float duration, float beat_duration, int divisions=1);
        void addMeasure(int divisions=1, int key=0, int beats=4, int beat_type=4, bool treble=true);

        std::vector<std::pair<std::string,std::string> > strings;
        std::string tuning;
};
#endif // GUITAR_TAB_CONVERTER_H
