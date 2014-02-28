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

#include "note.h"
#include "recorder.h"

void Recorder::Start(float time)
{
    recording = true;
    start_time = time;
}

void Recorder::Stop(Measure &measure, float time)
{
    if (recording)
    {
        Note note(pitches[pitches.size() / 2], 127, time - start_time, start_time);
        measure.AddNote(note);
        pitches.clear();
        recording = false;
    }
}

void Recorder::Update(int pitch)
{
    if (recording)
        pitches.push_back(pitch);
}