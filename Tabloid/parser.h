#ifndef PARSER_H
#define PARSER_H

#include <aubio/aubio.h>
#include <aubio/sndfileio.h>

class Parser
{
private:
    aubio_sndfile_t *aubio_file_;
public:
    Parser();
    // PitchDetection current implementation is just a test example.
    int PitchDetection();
    void GiveFile ();
};


#endif // PARSER_H
