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

#include "sheet_music.h"
#include "measure.h"

using namespace std;

//Default constructor
SheetMusic::SheetMusic()
{
}

//Instantiation with a vector of measures to fill the sheet.
SheetMusic::SheetMusic(vector<Measure> new_measures)
{
    AddMeasures(new_measures);
}

//Add one measure
void SheetMusic::AddMeasure(Measure new_measure)
{
    measures.push_back(new_measure);
}

//Add several measures in vector form
void SheetMusic::AddMeasures(vector<Measure> new_measures)
{
    for (vector<Measure>::iterator it = new_measures.begin();
         it != new_measures.end(); it++)
    {
        measures.push_back(*it);
    }
}

//Remove the last measure in the SheetMusic
Measure SheetMusic::RemoveLastMeasure()
{
    Measure ret = measures[measures.size()-1];
    measures.pop_back();
    return ret;
}

//Remove the first measure in the SheetMusic
Measure SheetMusic::RemoveHeadMeasure()
{
    Measure head_measure = measures[0];
    measures.erase(measures.begin());
    return head_measure;
}

vector<Measure> SheetMusic::GetAllMeasures()
{
    return measures;
}

void SheetMusic::reset()
{
    measures.clear();
}

