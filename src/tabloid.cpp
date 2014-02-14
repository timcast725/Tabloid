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

#include "converter.h"
#include "parser.h"
#include "sheet_music.h"

#include <iostream>

int main(int argc, char *argv[])
{
    SheetMusic music;
    Parser parse;
    parse.Parse("Bb.wav", 4, music);
    Converter convert;
    convert.Convert("Bb.xml", music);
    // music.reset();
    // parse.Parse("sdd_test.wav", music);
    return 1;
}
