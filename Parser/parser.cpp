#include "parser.h"

Parser::Parser()
{
    aubio_file_ = new_aubio_sndfile_ro("../Bb.wav");
}

int Parser::PitchDetection()
{
    // I don't know what these numbers mean, I just found them as
    // defaults in aubio. I think you're supposed to change them when
    // you load a file.
    uint_t buffer_size = 1024;
    uint_t hop_size = buffer_size / 4;
    uint_t samplerate = 44100;
    uint_t channels = 1;

    // This gives results in frequency.
    // Other options are MIDI, cent, and frequency bin.
    aubio_pitchdetection_mode mode = aubio_pitchm_freq;

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
    aubio_pitchdetection_type type = aubio_pitch_yinfft;

    fvec_t *input = new_fvec(hop_size, channels);
    aubio_pitchdetection_t *output;
    output = new_aubio_pitchdetection(buffer_size, hop_size, channels,
                                      samplerate, type, mode);
    for (int i = 0; i < 1000; i++)
        aubio_pitchdetection(output, input);

    // Must destroy aubio objects once done.
    del_aubio_pitchdetection(output);
    del_fvec(input);
    aubio_cleanup();

    return 0;
}
