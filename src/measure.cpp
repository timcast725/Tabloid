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

#include "measure.h"
#include "note.h"

using namespace std;

//Default constructor does nothing at the moment.
Measure::Measure()
{
}

Measure::Measure(vector<Note> new_notes)
{
    AddNotes(new_notes);
}

//Add one note
void Measure::AddNote(Note new_note)
{
    notes.push_back(new_note);
}

//Add several notes in vector form
void Measure::AddNotes(vector<Note> new_notes)
{
    for (vector<Note>::iterator it = new_notes.begin();
         it != new_notes.end(); it++)
    {
        notes.push_back(*it);
    }
}

//Remove the last note in the measure
Note Measure::RemoveLastNote()
{
    Note last = notes[notes.size()-1];
    notes.pop_back();
    return last;
}
//Remove the first note in the measure
Note Measure::RemoveHeadNote()
{
    Note head_note = notes[0];
    notes.erase(notes.begin());
    return head_note;
}

void Measure::clear()
{
    notes.clear();
}

vector<Note> Measure::GetAllNotes() const
{
    return notes;
}
