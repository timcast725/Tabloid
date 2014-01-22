#include "parser.h"
#include "sheet_music.h"
#include <iostream>

int main(int argc, char *argv[])
{
    SheetMusic music;
    Parser parse;
    parse.Parse("Bb.wav", music);
    music.reset();
    parse.Parse("sdd_test.wav", music);
    return 1;
}
