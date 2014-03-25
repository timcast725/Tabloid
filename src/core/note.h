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

//The Note class holds basic information about a single music note.
//This includes its pitch (related to frequency), velocity (related to amplitude),
//and duration (how long the note plays for) in milliseconds.
//
//In the future, information about the 'flavor' of the note can be added,
//such as common music markings.

#ifndef NOTE_H
#define NOTE_H

class Note
{

public:
    //Constructor
    Note(int p, int v, float d, float s);
    Note();

    int getPitch();
    int getVelocity();
    float getDuration();
    float getstart();

private:
    int pitch;
    int velocity;
    // duration in seconds
    float duration;
    // start time in seconds
    float start;

};

#endif // NOTE_H
