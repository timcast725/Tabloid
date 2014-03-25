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

#ifndef SHEET_MUSIC_H
#define SHEET_MUSIC_H

#include "measure.h"

using namespace std;

class SheetMusic
{

public:
    //Constructor
    SheetMusic();
    //Instantiate the sheet music with measures
    SheetMusic(vector<Measure> new_measures);
    //Adds a single measure to the end of the sheet
    void addMeasure(Measure new_measure);
    //Adds a vector of measures to the end of the sheet
    void addMeasures(vector<Measure> new_measures);
    //Removes a single measure from the end of the sheet
    //and returns the measure removed.
    Measure removeLastMeasure();
    //Removes a single measure from the beginning of the sheet
    //and returns the measure removed.
    Measure removeHeadMeasure();

    //Accessor for measure vector
    vector<Measure> getAllMeasures() const;

    void reset();

private:
    //Vector of measures representing the music
    vector<Measure> measures;

};

#endif // SHEET_MUSIC_H
