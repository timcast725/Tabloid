// Written by Gary Lu

#include "parser.h"
#include <iostream>
#include <sndfile.h>

void Parser::Parse(const char *file_name)
{
    AubioInit(file_name);
    AubioProcess();
    AubioDelete();
}

void Parser::AubioInit(const char *file_name)
{
    // Defaults that aubio uses.
    buffer_size_ = 1024;
    overlap_size_ = 256;
    pitch_ = 0;
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
    uint_t channels = aubio_sndfile_channels(aubio_file_);
    samplerate_ = aubio_sndfile_samplerate(aubio_file_);
    input_buffer_ = new_fvec(overlap_size_, channels);

    // Create objects that aubio will use.
    fft_grain_ = new_cvec(buffer_size_, channels);
    aubio_pitchdetection_type pitch_type = aubio_pitch_yinfft;
    aubio_pitchdetection_mode pitch_mode = aubio_pitchm_freq;
    pitch_detection_ = new_aubio_pitchdetection(buffer_size_ * 4,
                                                overlap_size_, channels,
                                                samplerate_, pitch_type,
                                                pitch_mode);
    aubio_pitchdetection_set_yinthresh(pitch_detection_, 0.7);
    pvoc_ = new_aubio_pvoc(buffer_size_, overlap_size_, channels);
    smpl_t threshold = 0.3;
    peak_ = new_aubio_peakpicker(threshold);
    aubio_onsetdetection_type onset_type = aubio_onset_kl;
    onset_detection_ = new_aubio_onsetdetection(onset_type, buffer_size_,
                                                channels);
    onset_ = new_fvec(1, channels);
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
        frames_++;
        frames_read = aubio_sndfile_read(aubio_file_, overlap_size_,
                                          input_buffer_);
    }

    std::cout << "Processed " << frames_ << " frames of " << buffer_size_ <<
                 " samples." << std::endl;
}

void Parser::AubioDelete()
{
    del_aubio_sndfile(aubio_file_);
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
                    // Get and send new note.
                    SendNoteOn(pitch_, 127 + (int) cur_level_);
                    cur_note_ = pitch_;
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
