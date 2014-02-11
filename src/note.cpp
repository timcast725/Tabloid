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

#include "note.h"

//Constructor
Note::Note(int p, int v, int d, int s)
{
    pitch = p;
    velocity = v;
    duration = d;
    start = s;
}

Note::Note()
{

}

int Note::GetPitch()
{
    return pitch;
}

int Note::GetVelocity()
{
    return velocity;
}

int Note::GetDuration()
{
    return duration;
}

int Note::GetStart()
{
    return start;
}
