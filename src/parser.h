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
#include <aubio.h>
#include <sndfileio.h>
#include <vector>

class Parser
{
private:
    Measure measure_;
    std::vector<smpl_t> pitches_;
    aubio_sndfile_t *aubio_file_;
    aubio_pitchdetection_t *pitch_detection_;
    aubio_onsetdetection_t *onset_detection_;
    aubio_tempo_t *beat_;
    aubio_pvoc_t *pvoc_;
    aubio_pickpeak_t *peak_;
    cvec_t *fft_grain_;
    fvec_t *input_buffer_;
    fvec_t *onset_;
    fvec_t *tempo_;
    uint_t samplerate_;
    uint_t buffer_size_;
    uint_t overlap_size_;
    smpl_t pitch_;
    unsigned int pos_;
    int frames_;
    int beats_;
    bool is_onset_;
    bool is_tactus_;

public:
    Parser();
    void Parse(const char *file_name, SheetMusic &sheet);

private:
    // Initializes member variables according to the input file.
    void AubioInit(const char *file_name);
    // Process the entire file. Should be called after AubioInit.
    void AubioProcess();
    // Deallocate stuff, should be called when done with a file.
    void AubioDelete();
};


#endif // PARSER_H
