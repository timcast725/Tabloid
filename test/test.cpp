#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

std::string midiToNote(int pitch)
{
    int octave = pitch / 12;
    std::string note;
    switch (pitch % 12)
    {
        case 0:
            note = "C";
            break;
        case 1:
            note = "C#";
            break;
        case 2:
            note = "D";
            break;
        case 3:
            note = "Eb";
            break;
        case 4:
            note = "E";
            break;
        case 5:
            note = "F";
            break;
        case 6:
            note = "F#";
            break;
        case 7:
            note = "G";
            break;
        case 8:
            note = "Ab";
            break;
        case 9:
            note = "A";
            break;
        case 10:
            note = "Bb";
            break;
        case 11:
            note = "B";
            break;
    }

    std::stringstream ss;
    ss << octave;
    note += ss.str();

    return note;
}

std::string frequencyToNote(double frequency)
{
    if (frequency < 0)
        return "none";

    int pitch = 57.50 + 12 * log2(frequency / 440.0);
    return midiToNote(pitch);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << "yo put in a file name yo\n";
        return 1;
    }
    std::string filename = argv[1];

    // Hard coding shiz
    std::string pitch = "python aubiopitch.py -i ";
    std::string target = " > test.txt";
    pitch += filename;
    pitch += target;
    system(pitch.c_str());

    // Frequencies
    std::ifstream output;
    output.open("test.txt");
    double tick;
    double frequency;
    std::vector<double> frequencies;
    std::vector<double> times;
    while (output >> tick >> frequency)
    {
        if (frequency > 0)
        {
            frequencies.push_back(frequency);
            times.push_back(tick);
        }
    }
    output.close();

    // Beats
    std::string onset = "python aubiocut.py -i ";
    onset += filename;
    onset += target;
    system(onset.c_str());
    output.open("test.txt");
    std::string note;
    int i = 0;
    double old_tick = 0.0;
    double average;
    double count;
    while (output >> tick)
    {
        average = 0.0;
        count = 0.0;
        while (times[i] < tick)
        {
            average += frequencies[i];
            i++;
            count += 1.0;
        }

        if (count > 0.0)
        {
            average /= count;
            note = frequencyToNote(average);
            std::cout << note << " at " << old_tick << " seconds " << std::endl;
        }
        old_tick = tick;
    }
    average = 0.0;
    count = 0.0;
    for (; i < times.size(); i++)
    {
        average += frequencies[i];
        count += 1.0;
    }

    if (count > 0.0)
    {
        average /= count;
        note = frequencyToNote(average);
        std::cout << note << " at " << old_tick << " seconds " << std::endl;
    }

    // SF_INFO info;
    // SNDFILE *wav = sf_open(argv[1], SFM_READ, &info);

    return 0;
}