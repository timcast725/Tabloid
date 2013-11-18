// Written by Zev Battad
//

#ifndef CONVERTER_H
#define CONVERTER_H

#include "sheet_music.h"

class Converter
{
public:
    Converter(){};
    virtual bool Convert(SheetMusic sheet) = 0;

private:
};


#endif // CONVERTER_H
