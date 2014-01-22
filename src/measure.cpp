#include "measure.h"
#include "note.h"

using namespace std;

//Default constructor does nothing at the moment.
Measure::Measure()
{
}

Measure::Measure(vector<Note> new_notes)
{
    AddNotes(new_notes);
}

//Add one note
void Measure::AddNote(Note new_note)
{
    notes.push_back(new_note);
}

//Add several notes in vector form
void Measure::AddNotes(vector<Note> new_notes)
{
    for (vector<Note>::iterator it = new_notes.begin();
         it != new_notes.end(); it++)
    {
        notes.push_back(*it);
    }
}

//Remove the last note in the measure
Note Measure::RemoveLastNote()
{
    Note last = notes[notes.size()-1];
    notes.pop_back();
    return last;
}
//Remove the first note in the measure
Note Measure::RemoveHeadNote()
{
    Note head_note = notes[0];
    notes.erase(notes.begin());
    return head_note;
}

void Measure::clear()
{
    notes.clear();
}

vector<Note> Measure::GetAllNotes()
{
    return notes;
}
