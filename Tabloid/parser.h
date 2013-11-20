// Written by Gary Lu

#ifndef PARSER_H
#define PARSER_H

#include "sheet_music.h"
#include <aubio.h>
#include <sndfileio.h>

class Parser
{
private:
    Measure measure_;
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
    smpl_t curr_note_;
    smpl_t curr_level_;
    unsigned int pos_;
    int frames_;
    int last_pitch_;
    int last_velocity_;
    int beats_;
    float last_time_;
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
    // Sends the note to sheet music.
    // @param pitch The pitch to print in frequency.
    // @param velocity The velocity of the note, 0 if silence.
    void SendNoteOn(int pitch, int velocity);
};


#endif // PARSER_H
