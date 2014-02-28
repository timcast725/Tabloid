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
    void Start(float time);
    void Stop(Measure &measure, float time);
    void Update(int pitch);
private:
    bool recording;
    float start_time;
    std::vector<int> pitches;
};

#endif
