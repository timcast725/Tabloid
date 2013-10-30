#ifndef SHEET_MUSIC_H
#define SHEET_MUSIC_H

#import "measure.h"

using namespace std;

class SheetMusic
{

public:
    //Constructor
    SheetMusic();
    //Instantiate the sheet music with measures
    SheetMusic(vector<measure> new_measures);
    //Adds a single measure to the end of the sheet
    void AddMeasure(measure new_measure);
    //Adds a vector of measures to the end of the sheet
    void AddMeasures(vector<measure> new_measures);
    //Removes a single measure from the end of the sheet
    //and returns the measure removed.
    Measure RemoveLastMeasure();
    //Removes a single measure from the beginning of the sheet
    //and returns the measure removed.
    Measure RemoveHeadMeasure();

    //Accessor for measure vector
    vector<Measure> GetAllMeasures();

private:
    //Vector of measures representing the music
    vector<Measure> measures;

};

#endif // SHEET_MUSIC_H
