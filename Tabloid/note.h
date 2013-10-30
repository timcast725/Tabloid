//Written by Zev Battad
//The Note class holds basic information about a single music note.
//This includes its pitch (related to frequency), velocity (related to amplitude),
//and duration (how long the note plays for) in milliseconds.
//
//In the future, information about the 'flavor' of the note can be added,
//such as common music markings.

#ifndef NOTE_H
#define NOTE_H

class Note
{

public:
    //Constructor
    Note(int p, int v, int d);

    int pitch;
    int velocity;
    //Duration is a millisecond value
    int duration;
};

#endif // NOTE_H
