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
    for (auto &new_note : new_notes)
    {
        notes.push_back(new_note);
    }
}

//Remove the last note in the measure
Note Measure::RemoveLastNote()
{
    return notes.pop_back();
}
//Remove the first note in the measure
Note Measure::RemoveHeadNote()
{
    Note head_note = notes[0];
    notes.erase(0);
    return head_note;
}

vector<Note> Measure::GetAllNotes()
{
    return notes;
}
