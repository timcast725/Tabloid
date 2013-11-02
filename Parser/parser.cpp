#include "parser.h"

Parser::Parser()
{
    aubio_file = new_aubio_sndfile_ro("../Bb.wav");
}
