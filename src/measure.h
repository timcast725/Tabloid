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


//The Measure class represents a measure of music. It holds multiple notes.
//

#ifndef MEASURE_H
#define MEASURE_H

#include "note.h"
#include <vector>

using namespace std;

class Measure
{

public:
    //Default Constructor
    Measure();
    //Instantiate the measure with notes
    Measure(vector<Note> new_notes);
    //Adds a single note to the end of the measure
    void addNote(Note new_note);
    //Adds a vector of notes to the end of the measure
    void addNotes(vector<Note> new_notes);
    //Removes a single note from the end of the measure
    //and returns the note removed.
    Note removeLastNote();
    //Removes a single note from the beginning of the measure
    //and returns the note removed.
    Note removeHeadNote();

    //Remove all notes from measure
    void clear();

    //Accessor method for vector of notes in this measure
    vector<Note> getAllNotes() const;

    float getBeat() { return beat; }
    void setBeat(float b) { beat = b; }

private:
    //Holds all the notes in this measure
    vector<Note> notes;

    // Beat duration is the number of seconds a beat takes
    float beat;
};

#endif // MEASURE_H
