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

#include <vector>

class Recorder
{
public:
    Recorder() { recording = false; }
    /**
     * Start recording.
     * @param time The time the recording started at.
     */
    void start(float time);
    /**
     * Stop the recording and add the note to the measure.
     * @param measure The measure to add the note to.
     * @param time The time in seconds the recorder stopped at.
     */
    void stop(Measure &measure, float time);
    /**
     * To be called every frame.
     * @param pitch The midi note currently being played.
     */
    void update(int pitch);
private:
    bool recording;
    float start_time;
    std::vector<int> pitches;
};

#endif
