// Written by Zev Battad

#include "midi_converter.h"

#include <time.h>
#include <string>
#include <sstream>

#include "MidiFile.h"

#include "sheet_music.h"
#include "note.h"

using ::std::cout;

MidiConverter::MidiConverter()
{
    MidiConverter("");
}
MidiConverter::MidiConverter(string outfile_name)
{
    //If a file name has not been specified, use the default file name
    if (outfile_name.compare("") == 0 )
        SetFileNameToDefault();
    else
        set_output_file_name(outfile_name);
}

void MidiConverter::set_output_file_name(string new_name)
{
    stringstream temp_stream;
    temp_stream << new_name << ".mid";
    output_file_name_ = temp_stream.str();
    cout << "\nsetting output file name to " << output_file_name_;
}
string MidiConverter::output_file_name()
{
    return output_file_name_;
}

void MidiConverter::SetFileNameToDefault()
{
    string default_file_name = "Tabloid_output_" + to_string(time(nullptr));
    set_output_file_name(default_file_name);
}

// Convert takes a sheet music object and writes a MIDI file based off of it.
// The function will return true when the conversion is successful, false if not.
bool MidiConverter::Convert(SheetMusic sheet)
{   cout << "\nIn convert, output_file_name_ = " << output_file_name_;
    if (output_file_name_.compare("") == 0)
    {
        cout << "\nMIDI File name empty, cannot convert";
        cout << "\n setting default midi file name";
        SetFileNameToDefault();
        cout << "\nIn convert if, output_file_name_ = " << output_file_name_;
    }
    MidiFile temp_file;
    temp_file.absoluteTime();
    Array<uchar> temp_event;
    temp_event.setSize(3); //midi event data must be of size 3

    vector<Measure> temp_measures = sheet.GetAllMeasures();
    vector<Note> temp_notes;

    // Calculated how many milliseconds each midi tick takes based off of the
    // MidiFile's TicksPerQuarter. TPQ represents a 500ms note given Midi's
    // default Beats per Measure of 120.
    // NOTE: This calculation will not be valid if the BPM is changed.
    int ticks_per_quarter = temp_file.getTicksPerQuarterNote();
    double milliseconds_per_tick = ((double)500/(double)ticks_per_quarter);

    int event_time;

    cout << "\nMIDI_FILE_INFO";
    cout << "\nticks_per_quarter: " << ticks_per_quarter;
    cout << "\nmilliseconds_per_tick: " << milliseconds_per_tick;

    for(vector<Measure>::iterator sheet_iter = temp_measures.begin();
        sheet_iter != temp_measures.end(); sheet_iter++)
    {
        //Iterate through measure
        temp_notes = sheet_iter->GetAllNotes();
        for(vector<Note>::iterator measure_iter = temp_notes.begin();
            measure_iter != temp_notes.end(); measure_iter++)
        {
            temp_event[0] = 0x90; //turn the note on
            temp_event[2] = measure_iter->GetVelocity();
            temp_event[1] = measure_iter->GetPitch();
            cout << "\nNEW NOTE: setting pitch " << measure_iter->GetPitch();
            event_time =
                (int)((measure_iter->GetStart())/milliseconds_per_tick+0.5);
            //Add the event (turn on velocity, pitch) to the midi file at time event_time
            temp_file.addEvent(0, event_time, temp_event);
            cout << "\nadding event at " << event_time;
            //Increment event_time by the duration of the note
            event_time +=
                (int)((measure_iter->GetDuration())/milliseconds_per_tick + 0.5);
            temp_event[0] = 0x80; //turn the pitch and velocity off
            //Add the event (turn off velocity, pitch) to the midi file at time event_time
            temp_file.addEvent(0, event_time, temp_event);
            cout << "\nturning event off at " << event_time;
        }
    }

    cout << "\nwriting midi file " << output_file_name_;
    temp_file.write(output_file_name_.c_str()); //write the midi file we just made

    return true;
}
