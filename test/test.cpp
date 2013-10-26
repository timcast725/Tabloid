#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <aubio/aubio.h>
// #include <python2.7/Python.h>

std::string frequencyToNote(float frequency)
{
    if (frequency < 0)
        return "none";

    int pitch = 57.50 + 12 * log2(frequency / 440.0);
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

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << "yo put in a file name yo\n";
        return 1;
    }
    // Hard coding shiz
    std::string command = "python aubiopitch.py -i ";
    std::string filename = argv[1];
    command += filename;
    command += " > test.txt";
    // Py_Initialize();
    system(command.c_str());

    std::ifstream frequency_text;
    frequency_text.open("test.txt");
    float old_tick = 0;
    float tick;
    float frequency;
    std::string note;
    while (frequency_text >> tick >> frequency)
    {
        if (frequency > 0 && note != frequencyToNote(frequency))
        {
            note = frequencyToNote(frequency);
            std::cout << tick - old_tick << " " << note << " (" << frequency << ")" << std::endl;
            old_tick = tick;
        }
    }

    return 0;
}