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
#include <iostream>

Recorder::Recorder(SheetMusic *s, float end)
{
    sheet = s;
    recording = false;
    measure_end = end;
    time_left = end;
}

void Recorder::start(float time)
{
    recording = true;
    start_time = time;
}

void Recorder::stop(float stop_time)
{
    if (recording)
    {
        Note note(pitches[pitches.size() / 2], 127, stop_time - start_time, start_time);
        time_left -= stop_time - start_time;
        // Add the note in if it does not extend over halfway into the next measure.
        float limit = stop_time - start_time;
        limit /= 2;
        if (time_left < -limit)
        {
            measure.setBeat(60 / bpm);
            sheet->addMeasure(measure);
            measure.clear();
            time_left = measure_end - stop_time;
        }
        measure.addNote(note);
        pitches.clear();
        recording = false;
    }
}

void Recorder::update(int pitch)
{
    if (recording)
        pitches.push_back(pitch);
}

void Recorder::load(float time, float b, int beats_per_measure)
{
    bpm = b;
    measure_end = time + 60 / bpm * beats_per_measure;
}

void Recorder::end()
{
    measure.setBeat(60 / bpm);
    sheet->addMeasure(measure);
}