// Written by Zev Battad
//

#ifndef CONVERTER_H
#define CONVERTER_H

#include "sheet_music.h"
#include "MidiFile.h"

class Converter
{
private:

public:
    Converter();
    bool convert(SheetMusic sheet);
};


#endif // CONVERTER_H
