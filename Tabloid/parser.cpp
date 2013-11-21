// Written by Gary Lu

#include "parser.h"
#include <iostream>
#include <sndfile.h>
#include <sys/stat.h>

Parser::Parser()
{
    beats_ = 0;
}

void Parser::Parse(const char *file_name, SheetMusic &sheet)
{
    AubioInit(file_name);
    AubioProcess();
    AubioDelete();
    sheet.AddMeasure(measure_);
}

void Parser::AubioInit(const char *file_name)
{
    // Defaults that aubio uses.
    buffer_size_ = 1024;
    overlap_size_ = 256;
    pitch_ = 0;
    pos_ = 0;
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
    pitch_detection_ = new_aubio_pitchdetection(buffer_size_ * 4,
                                                overlap_size_, channels,
                                                samplerate_, aubio_pitch_yinfft,
                                                aubio_pitchm_freq);
    aubio_pitchdetection_set_yinthresh(pitch_detection_, 0.7);
    pvoc_ = new_aubio_pvoc(buffer_size_, overlap_size_, channels);
    peak_ = new_aubio_peakpicker(0.3);
    onset_detection_ = new_aubio_onsetdetection(aubio_onset_kl, buffer_size_,
                                                channels);
    onset_ = new_fvec(1, channels);
    beat_ = new_aubio_tempo(aubio_onset_kl, buffer_size_,
                                   overlap_size_, channels);
    tempo_ = new_fvec(2, channels);
}

void Parser::AubioProcess()
{
    std::cout << "Processing...\n";
    frames_ = 0;
    float time = 0;
    float last_time = 0;
    smpl_t midi_pitch = 0;
    smpl_t pitch = 0;
    int velocity = 0;
    bool first = true;
    int frames_read = aubio_sndfile_read(aubio_file_, overlap_size_,
                                          input_buffer_);
    while ((signed) overlap_size_ == frames_read)
    {
        is_onset_ = 0;
        // Go through frames
        for (unsigned int frame = 0; frame < overlap_size_; frame++)
        {
            // FFT gonna happen here
            if (pos_ == overlap_size_ - 1)
            {
                // Block loop
                // Stuff for notes
                aubio_pvoc_do(pvoc_, input_buffer_, fft_grain_);
                aubio_onsetdetection(onset_detection_, fft_grain_, onset_);
                is_onset_ = aubio_peakpick_pimrt(onset_, peak_);
                pitch_ = aubio_pitchdetection(pitch_detection_, input_buffer_);
                // If silent, curlevel is either negative or 1.
                if (is_onset_)
                {
                    time = (float) frames_ * overlap_size_ / (float) samplerate_;
                    if (!first)
                    {
                        pitch = pitches_[pitches_.size() / 2];
                        if (pitch != 0)
                        {
                            midi_pitch = (int) (aubio_freqtomidi(pitch) + 0.5);
                            std::cout << "pitch: " << midi_pitch <<
                                         "\t\ttime: " << last_time << std::endl;
                            velocity = 127;
                        }
                        else
                        {
                            std::cout << "rest\t\ttime: " << last_time << std::endl;
                            midi_pitch = 0;
                            velocity = 0;
                        }
                        int duration = (int) (1000 * (time - last_time) );
                        int start = (int) (1000 * last_time);
                        Note note(midi_pitch, velocity, duration, start);
                        measure_.AddNote(note);
                        pitches_.clear();
                    }
                    last_time = time;
                    first = false;
                }
                if (!first)
                    pitches_.push_back(pitch_);

                // Stuff for tempo
                aubio_tempo(beat_, input_buffer_, tempo_);
                // [0][0] is beat, [0][1] is onset
                if (tempo_->data[0][0] == 1)
                    beats_++;
                pos_ = -1;
            }
            pos_++;
        }
        frames_++;
        frames_read = aubio_sndfile_read(aubio_file_, overlap_size_,
                                          input_buffer_);
    }

    float average_duration = ((float) frames_ * overlap_size_ / (float) samplerate_);
    average_duration /= (float) (beats_ - 1);
    int tempo = (int) (1.0 / average_duration * 60.0 + 0.5);
    if (tempo > 0)
        std::cout << "Tempo: " << tempo << std::endl;

    std::cout << "Processed " << frames_ << " frames of " << buffer_size_ <<
                 " samples." << std::endl;
}

void Parser::AubioDelete()
{
    del_aubio_sndfile(aubio_file_);
    del_aubio_pitchdetection(pitch_detection_);
    del_aubio_onsetdetection(onset_detection_);
    del_aubio_peakpicker(peak_);
    del_aubio_pvoc(pvoc_);
    del_aubio_tempo(beat_);
    del_fvec(input_buffer_);
    del_fvec(onset_);
    del_fvec(tempo_);
    del_cvec(fft_grain_);
    aubio_cleanup();
}
