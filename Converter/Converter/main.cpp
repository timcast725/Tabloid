// Copyright 2013 Zev Battad

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

#include <QCoreApplication>
#include "note.h"
#include "measure.h"
#include "sheet_music.h"
#include "converter.h"
#include "midi_converter.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int melody[50]  = {72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1};
    int mrhythm[50] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,-1};

    vector<Note> test_notes;
    //Note temp_note;
    //write the notes, put them in measures, and put the measures
    //in a sheet music file
    int duration = 500; //each note will be half a second long
    int velocity = 68; //each note will have a velocity of 68
    int start_time = 0; //start time will increment such that

    //cout << "Duration: " + to_string(duration) + " velocity: " + to_string(velocity) + " start_time " + to_string(start_time) + "\n";

    //the midi will play one note at a time
    //All notes store timing information as millisecond values
    int i = 0;
    while(melody[i] >= 0)
    {
        //temp_note = new Note(melody[i], 68, duration, start_time);
        test_notes.push_back(*(new Note(melody[i], 68, duration*mrhythm[i], start_time)));
        start_time += duration*mrhythm[i];
        i++;
    }

    vector<Note> test_notes_one_ms;
    test_notes_one_ms.push_back(*(new Note(50, 68, 1000, 0)));

    Measure test_measure;
    test_measure.AddNotes(test_notes);
    SheetMusic test_sheet;
    test_sheet.AddMeasure(test_measure);

    vector<Measure>::iterator sheet_iter = test_sheet.GetAllMeasures().begin();

    MidiConverter test_converter = MidiConverter("TestOutput");
    cout << "\ntest_converter's output_file_name_: " << test_converter.output_file_name();
    test_converter.Convert(test_sheet);
    MidiConverter test_converter2 = MidiConverter();
    cout << "\ntest_converter2's output_file_name_: " << test_converter2.output_file_name();
    test_converter2.Convert(test_sheet);

    //midi events are formatted as an unsigned character array of size 3:
    //0: 0x90 to store a note as ON
    //0: 0x80 turns the note OFF
    //1: frequency of note (1-127)
    //2: velocity of note (1-127)
    //int addEvent(int aTrack, int aTime, Array<uchar>& midiData);
    //   Add an event to the end of a MIDI track.
    // aTrack is the tracak number, aTime is the time at which the action takes place
    //default 48 ticks per quarter note

    MidiFile test_output_file;
    test_output_file.absoluteTime(); //file will use absolute time instead of
                                    //delta time

    Array<uchar> temp_event;
    temp_event.setSize(3); //an event must be of size 3


    int ticks_per_quarter = test_output_file.getTicksPerQuarterNote(); //Default value in MIDI
    //double beats_per_minute = temp_event.getTempoBPM();

    cout << "\nMIDI_FILE_INFO";
    cout << "\nticks_per_quarter: " << ticks_per_quarter;
    //cout << "\nbpm: " << beats_per_minute;


    temp_event[2] = 64; //the velocity of each note will be this value

    int event_time = 0; //time at which this event takes place
    i = 0;

/*    while(melody[i] >= 0)
    {
        temp_event[0] = 0x90; //turn the note on
        temp_event[1] = melody[i]; //set the note's pitch
        cout << "\nNEW NOTE: setting pitch " << melody[i];
        //Add the event (turn on velocity, pitch) to the test file at time event_time
        test_output_file.addEvent(0, event_time, temp_event);
        cout << "\nadding event at " << event_time;
        //Increment event_time by one quarter note (default 48 ticks)
        event_time += 48;
        temp_event[0] = 0x80; //turn the pitch and velocity off
        //Add the event (turn off velocity, pitch) to the test file at time event_time
        test_output_file.addEvent(0, event_time, temp_event);
        cout << "\nturning event off at " << event_time;
        i++;
    }*/

    //test_output_file.write("TestMidi.mid"); //write the midi file we just made

    return a.exec();
}
