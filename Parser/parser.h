#ifndef PARSER_H
#define PARSER_H

#include <aubio.h>
#include <sndfileio.h>
#include <sndfile.h>

// NOTE: I don't understand almost half of what's happening here.
// NOTE: Remember to get rid of unecessary stuff once this is working
// NOTE: I don't think anything involving output is necessary?
// NOTE: Way too many members, get rid of some.

class Parser
{
private:
    // Pointer to snd file, contains *handle, samplerate, channels, and format.
    aubio_sndfile_t *aubio_file_;

    // Pitch detection object
    aubio_pitchdetection_t *pitch_detection_;

    // Type of pitch detection to use.
    // YIN is a time-domain approach that uses autocorrelation.
    //     Fast, requires a minimum audio length, only one note at a time.
    //     De Cheveigné, A., Kawahara, H. (2002) "YIN, a fundamental frequency
    //     estimator for speech and music", J. Acoust. Soc. Am. 111, 1917-1930.
    // mcomb uses a multi-comb filter, spectral flattening, peak histogramming.
    //     I think it uses both time and frequency domains.
    //     Juan-Pablo Bello. ``Towards the Automated Analysis of Simple
    //     Polyphonic Music''.  PhD thesis, Centre for Digital Music,
    //     Queen Mary University of London, London, UK, 2003.
    // schmitt uses something called a Schmitt trigger:
    //     http://en.wikipedia.org/wiki/Schmitt_trigger
    //     aubio implementation derived from http://delysid.org/tuneit.html
    // fcomb uses fast harmonic comb filter. It's a fequency domain algorithm.
    //     aubio implementation derived from http://delysid.org/tuneit.html
    // yinfft is Spectral YIN. It combines YIN with and FFT to use both
    //     a time and frequency domain approach. It's the default in aubio,
    //     because it's a whole chapter in aubio's thesis paper.
    aubio_pitchdetection_type pitch_type_;

    // The mode can be frequency, MIDI, cent, or frequency bin
    aubio_pitchdetection_mode pitch_mode_;

    // Onset detection stuff for the beginning of notes.
    aubio_onsetdetection_t *onset_detection_;
    aubio_onsetdetection_type onset_type_;

    // No clue what these are, but I can guess.
    aubio_pvoc_t *pvoc_;
    aubio_pickpeak_t *peak_;
    cvec_t *fft_grain_;
    fvec_t *input_buffer_;
    fvec_t *output_buffer_;
    fvec_t *onset_;

    // I think the median is used for better approximation.
    // Might not be necessary.
    // fvec_t *note_buffer_;
    // fvec_t *note_buffer2_;
    // uint_t median_;

    // Audio information.
    uint_t channels_;
    uint_t samplerate_;
    uint_t buffer_size_;
    uint_t overlap_size_;

    // Information to keep track of the parse.
    smpl_t pitch_;
    smpl_t threshold_;
    smpl_t cur_note_;
    smpl_t cur_level_;
    int frames_;
    bool is_onset_;

public:
    Parser();
    ~Parser();

    // Initializes member variables according to the input file.
    // Every time you want to parse a file, you must call this.
    // @param file_name The file Parser will parse.
    void AubioInit(const char *file_name);

    // Runs an aubio function on the aubio file.
    // Currently only AubioNotes available.
    // To see the notes in the file, you must call this after AubioInit.
    void AubioProcess();

private:
    int AubioNotes(int nframes);

    // Print out the MIDI note.
    // @param pitch The pitch to print, in frequency.
    // @param velocity The velocity of the note. 0 if silence.
    void SendNoteOn(int pitch, int velocity);
};


#endif // PARSER_H
