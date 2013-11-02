#ifndef PARSER_H
#define PARSER_H

#include <aubio/aubio.h>
#include <aubio/sndfileio.h>

// Right now this is empty and useless.
class Parser
{
private:
    aubio_sndfile_t *aubio_file;
public:
    Parser();
};


#endif // PARSER_H
