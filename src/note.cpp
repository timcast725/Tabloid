#include "note.h"

//Constructor
Note::Note(int p, int v, int d, int s)
{
    pitch = p;
    velocity = v;
    duration = d;
    start = s;
}

Note::Note()
{

}

int Note::GetPitch()
{
    return pitch;
}

int Note::GetVelocity()
{
    return velocity;
}

int Note::GetDuration()
{
    return duration;
}

int Note::GetStart()
{
    return start;
}
