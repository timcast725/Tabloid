// Copyright 2013, 2014 Gary Lu

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

#ifndef PARSER_H
#define PARSER_H

#include "sheet_music.h"
#include <aubio/aubio.h>

const int TEMPO_LOOP = 8;

class Parser
{
private:
    aubio_source_t *aubio_source;
    aubio_pitch_t *aubio_pitch;
    aubio_onset_t *aubio_onset;
    aubio_tempo_t *aubio_tempo;
    uint_t samplerate;
    uint_t hop_size;
    uint_t buffer_size;
    fvec_t *input_buffer;
    smpl_t silence_threshold;

public:
    Parser();
    bool Parse(char *file_name, int beats_per_measure, SheetMusic &sheet);

private:
    bool aubioInit(char *file_name);
    void aubioProcess(int beats_per_measure, SheetMusic &sheet);
    void aubioDelete();
};


#endif // PARSER_H
