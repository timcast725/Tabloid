#include "sheet_music.h"
#include "measure.h"

using namespace std;

//Default constructor
SheetMusic::SheetMusic()
{
}

//Instantiation with a vector of measures to fill the sheet.
SheetMusic::SheetMusic(vector<Measure> new_measures)
{
    AddMeasures(new_measures);
}

//Add one measure
void SheetMusic::AddMeasure(Measure new_measure)
{
    measures.push_back(new_measure);
}

//Add several measures in vector form
void SheetMusic::AddMeasures(vector<Measure> new_measures)
{
    for (vector<Measure>::iterator it = new_measures.begin();
         it != new_measures.end(); it++)
    {
        measures.push_back(*it);
    }
}

//Remove the last measure in the SheetMusic
Measure SheetMusic::RemoveLastMeasure()
{
    Measure ret = measures[measures.size()-1];
    measures.pop_back();
    return ret;
}

//Remove the first measure in the SheetMusic
Measure SheetMusic::RemoveHeadMeasure()
{
    Measure head_measure = measures[0];
    measures.erase(measures.begin());
    return head_measure;
}

vector<Measure> SheetMusic::GetAllMeasures()
{
    return measures;
}

void SheetMusic::reset()
{
    measures.clear();
}

