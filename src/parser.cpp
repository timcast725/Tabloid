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

#include "parser.h"
#include "measure.h"
#include <iostream>
#include <algorithm>
#include <sndfile.h>
#include <sys/stat.h>

Parser::Parser()
{
    // beats_ = 0;
    // Values to give the user the option to change in the future:
    // hop_size
    // buffer_size
    // pitch_tolerance (default 0)
    // pitch_unit
    // onset_threshold (default 0), tempo uses this too
    // silence_threshold (default -90), all use the same silence
    samplerate = 0;
    hop_size = 256;
    buffer_size = 2048;
}

bool Parser::Parse(char *file_name, int beats_per_measure, SheetMusic &sheet)
{
    if (!AubioInit(file_name))
    {
        std::cerr << "Failed to initialize" << std::endl;
        return false;
    }
    AubioProcess(beats_per_measure, sheet);
    AubioDelete();
    return true;
}

bool Parser::AubioInit(char *file_name)
{
    // Open the audio file.
    std::cout << "Opening file " << file_name << std::endl;
    aubio_source = new_aubio_source(file_name, 0, hop_size);
    if (!aubio_source)
    {
        std::cerr << "Could not open file " << file_name << std::endl;
        return false;
    }
    samplerate = aubio_source_get_samplerate(aubio_source);
    input_buffer =  new_fvec(hop_size);

    // Create detection objects.
    char_t *mode = (char_t *) "default";
    aubio_pitch = new_aubio_pitch(mode, buffer_size, hop_size, samplerate);
    aubio_onset = new_aubio_onset(mode, buffer_size, hop_size, samplerate);
    aubio_tempo = new_aubio_tempo(mode, buffer_size, hop_size, samplerate);
    char_t *unit = (char_t *) "midi";
    aubio_pitch_set_unit(aubio_pitch, unit);

    return true;
}

void Parser::AubioProcess(int beats_per_measure, SheetMusic &sheet)
{
    std::cout << "Processing...\n";
    int blocks = 0;
    uint_t read = hop_size;
    fvec_t *tempo_output = new_fvec(2);
    // Loop TEMPO_LOOP times for aubio beat detection to settle down.
    // Arbitrary amount, 4-6 seems to work, 8 to be safe.
    int i = 0;
    while (i < TEMPO_LOOP)
    {
        aubio_source_do(aubio_source, input_buffer, &read);
        aubio_tempo_do(aubio_tempo, input_buffer, tempo_output);
        // tempo_output[0] for beat detection, tempo_output[1] for onset
        if (fvec_get_sample(tempo_output, 0))
        {
            std::cout << "Beat at " << aubio_tempo_get_last_s(aubio_tempo)
                      << " seconds at " << aubio_tempo_get_bpm(aubio_tempo)
                      << " bpm\n";
            i++;
        }
        blocks++;
    }
    // Reset the audio file
    aubio_source_seek(aubio_source, 0);
    blocks = 0;
    read = hop_size;
    float first_beat_time = 0;
    fvec_t *onset_output = new_fvec(1);
    fvec_t *pitch_output = new_fvec(1);
    // Loop until the song is finished processing.
    while (read == hop_size)
    {
        aubio_source_do(aubio_source, input_buffer, &read);
        // Process pitch
        aubio_pitch_do(aubio_pitch, input_buffer, pitch_output);
        // Process onset block
        aubio_onset_do(aubio_onset, input_buffer, onset_output);
        if (fvec_get_sample(onset_output, 0))
        {
            // Onset detected
            std::cout << "Note " << fvec_get_sample(pitch_output, 0) << " at "
                      << aubio_onset_get_last_s(aubio_onset) << " seconds with level "
                      << 127 + aubio_level_detection(input_buffer, -90) << std::endl;
        }
        else
        {

        }
        // Process tempo
        aubio_tempo_do(aubio_tempo, input_buffer, tempo_output);
        if (fvec_get_sample(tempo_output, 0))
        {
            if (first_beat_time <= 0)
                first_beat_time = aubio_tempo_get_last_s(aubio_tempo);
            std::cout << "Beat at " << aubio_tempo_get_last_s(aubio_tempo) - first_beat_time
                      << " seconds\n";
        }
        blocks++;
    }
    std::cout << "Tempo: " << aubio_tempo_get_bpm(aubio_tempo) << std::endl;
    del_fvec(pitch_output);
    del_fvec(onset_output);
    del_fvec(tempo_output);
}

void Parser::AubioDelete()
{
    del_fvec(input_buffer);
    del_aubio_source(aubio_source);
    del_aubio_pitch(aubio_pitch);
    del_aubio_onset(aubio_onset);
    del_aubio_tempo(aubio_tempo);
    aubio_cleanup();
}
