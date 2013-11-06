#ifndef PARSER_H
#define PARSER_H

#include <string>

#include <aubio/aubio.h>
#include <aubio/sndfileio.h>
#include <sheet_music.h>

class Parser
{
private:
    aubio_sndfile_t *aubio_file_;
    SheetMusic m_sheetmusic;
public:
    Parser();

    init (std::string filename);
    // PitchDetection current implementation is just a test example.
    int PitchDetection();
};


#endif // PARSER_H
