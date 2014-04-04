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

#ifndef RECORDER_H
#define RECORDER_H

#include "measure.h"
#include "sheet_music.h"

#include <vector>

class Recorder
{
public:
    Recorder(SheetMusic *s, float end);
    // Start recording.
    // time is the time in seconds the recording started at.
    void start(float time);

    // Stop the recording and add the note.
    // measure is the measure to add the recorded note to.
    // time is the seconds the recorder ended on.
    void stop(float stop_time = 0);

    // Call this every frame when using the recorder.
    // pitch is added to a vector of pitches when this is recording.
    void update(int pitch);

    // After a certain number of beats that the parser determines,
    // load the times for the next measure.
    // time is the current time load is called
    // b is the beats per minute
    void load(float time, float b, int beats_per_measure);

    // Add the final measure into the sheet music.
    void end();

private:
    SheetMusic *sheet;
    Measure measure;
    bool recording;
    float start_time;
    float measure_end;
    float time_left;
    float bpm;
    std::vector<int> pitches;
};

#endif
