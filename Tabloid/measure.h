//Written by Zev Battad
//The Measure class represents a measure of music. It holds multiple notes.
//

#ifndef MEASURE_H
#define MEASURE_H

#include "note.h"
#include <vector>

using namespace std;

class Measure
{

public:
    //Default Constructor
    Measure();
    //Instantiate the measure with notes
    Measure(vector<Note> new_notes);
    //Adds a single note to the end of the measure
    void AddNote(Note new_note);
    //Adds a vector of notes to the end of the measure
    void AddNotes(vector<Note> new_notes);
    //Removes a single note from the end of the measure
    //and returns the note removed.
    Note RemoveLastNote();
    //Removes a single note from the beginning of the measure
    //and returns the note removed.
    Note RemoveHeadNote();

    //Accessor method for vector of notes in this measure
    vector<Note> GetAllNotes();

private:
    //Holds all the notes in this measure
    vector<Note> notes;
};

#endif // MEASURE_H
