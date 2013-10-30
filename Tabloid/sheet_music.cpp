#include "sheet_music.h"
#include "measure.h"

using namespace std;

//Default constructor
SheetMusic::SheetMusic()
{
}

//Instantiation with a vector of measures to fill the sheet.
SheetMusic::SheetMusic(vector<measure> new_measures)
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
    for (auto &new_measure : new_measures)
    {
        measures.push_back(new_measure);
    }
}

//Remove the last measure in the SheetMusic
measure SheetMusic::RemoveLastMeasure()
{
    return measures.pop_back();
}
//Remove the first measure in the SheetMusic
measure SheetMusic::RemoveHeadMeasure()
{
    Measure head_measure = measures[0];
    measures.erase(0);
    return head_measure;
}

vector<measure> SheetMusic::GetAllMeasures()
{
    return measures;
}

