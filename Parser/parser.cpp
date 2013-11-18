#include "parser.h"
#include <iostream>

Parser::Parser()
{
    // These are just defaults aubio uses.
    pitch_type_ = aubio_pitch_yinfft;
    pitch_mode_ = aubio_pitchm_freq;
    onset_type_ = aubio_onset_kl;
}

Parser::~Parser()
{
    SendNoteOn(cur_note_, 0);
    del_aubio_pitchdetection(pitch_detection_);
    del_aubio_onsetdetection(onset_detection_);
    del_aubio_peakpicker(peak_);
    del_aubio_pvoc(pvoc_);
    del_fvec(input_buffer_);
    del_fvec(onset_);
    del_cvec(fft_grain_);
    aubio_cleanup();
}

void Parser::AubioInit(const char *file_name)
{
    // Defaults that aubio uses.
    buffer_size_ = 1024;
    overlap_size_ = 256;
    pitch_ = 0;
    threshold_ = 0.3;
    cur_note_ = 0;
    cur_level_ = 0;
    frames_ = 0;
    is_onset_ = false;

    // Open the audio file with the SND library.
    aubio_file_ = new_aubio_sndfile_ro(file_name);
    if (!aubio_file_)
        std::cerr << "Could not open file\n";
    aubio_sndfile_info(aubio_file_);

    // Load audio file information into the Parser class.
    channels_ = aubio_sndfile_channels(aubio_file_);
    samplerate_ = aubio_sndfile_samplerate(aubio_file_);
    input_buffer_ = new_fvec(overlap_size_, channels_);
    // output_buffer_  = new_fvec(overlap_size_, channels_);

    // Create objects that aubio will use.
    fft_grain_ = new_cvec(buffer_size_, channels_);
    pitch_detection_ = new_aubio_pitchdetection(buffer_size_ * 4,
                                                overlap_size_, channels_,
                                                samplerate_, pitch_type_,
                                                pitch_mode_);
    aubio_pitchdetection_set_yinthresh(pitch_detection_, 0.7);
    pvoc_ = new_aubio_pvoc(buffer_size_, overlap_size_, channels_);
    peak_ = new_aubio_peakpicker(threshold_);
    onset_detection_ = new_aubio_onsetdetection(onset_type_, buffer_size_,
                                                channels_);
    onset_ = new_fvec(1, channels_);
}

void Parser::AubioProcess()
{
    std::cout << "Processing...\n";
    frames_ = 0;
    int frames_read = aubio_sndfile_read(aubio_file_, overlap_size_,
                                          input_buffer_);
    while ((signed) overlap_size_ == frames_read)
    {
        is_onset_ = 0;
        // AubioNotes should be swappable with other functions in the future.
        AubioNotes(overlap_size_);
        // std::cout << ":H:" << pitch_ << std::endl;
        frames_++;
        frames_read = aubio_sndfile_read(aubio_file_, overlap_size_,
                                          input_buffer_);
    }

    std::cout << "Processed " << frames_ << " frames of " << buffer_size_ <<
                 " samples." << std::endl;

    del_aubio_sndfile(aubio_file_);
    // AubioInit must be called each time.
}

int Parser::AubioNotes(int nframes)
{
    // pos keeps track of position, is frame % dsp block size
    unsigned int pos = 0;
    for (int frame = 0; frame < nframes; frame++)
    {
        // FFT gonna happen here
        if (pos == overlap_size_ - 1)
        {
            // Block loop
            aubio_pvoc_do(pvoc_, input_buffer_, fft_grain_);
            aubio_onsetdetection(onset_detection_, fft_grain_, onset_);
            is_onset_ = aubio_peakpick_pimrt(onset_, peak_);
            pitch_ = aubio_pitchdetection(pitch_detection_, input_buffer_);
            // If silent, curlevel is either negative or 1.
            if (is_onset_)
            {
                // Test for silence.
                if (cur_level_ == 1)
                {
                    is_onset_ = 0;
                    SendNoteOn(cur_note_, 0);
                }
                else
                {
                    // Kill old note.
                    SendNoteOn(cur_note_, 0);
                    // Get and send new note.
                    SendNoteOn(pitch_, 127 + (int) cur_level_);
                    cur_note_ = pitch_;

                    // Writing to output from audio example. Safe to not have?
                    // for (pos = 0; pos < overlap_size_; pos++)
                }
            }
            pos = -1;
        }
        pos++;
    }

    return 1;
}

void Parser::SendNoteOn(int pitch, int velocity)
{
    smpl_t mpitch = (int) (aubio_freqtomidi(pitch) + 0.5);
    float time = (float) frames_ * overlap_size_ / (float) samplerate_;
    if (velocity == 0)
        std::cout << "pitch: none" << "\t\ttime: " << time << std::endl;
    else
        std::cout << "pitch: " << mpitch << "\t\ttime: " << time << std::endl;
}
