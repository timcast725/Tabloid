// Copyright 2014 Gary Lu

// This file is part of Tabloid.

// Tabloid is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Tabloid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Tabloid.  If not, see <http://www.gnu.org/licenses/>.
//

#include "sheet_music_converter.h"
#include "converter.h"
#include "note.h"

#include <iostream>
#include <sstream>


SheetMusicConverter::SheetMusicConverter()
{
    measure_number = 1;

    last_key = -1;
    last_beats = -1;
    last_beat_type = -1;
    last_clef = true;
}

bool SheetMusicConverter::convert(const std::string &name, const SheetMusic &sheet)
{
    output.open(name.c_str(), ios::out | ios::trunc);

    // MusicXML header
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    output << std::endl;
    output << "<!DOCTYPE score-partwise PUBLIC" << std::endl;
    output << "\"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << std::endl;
    output << "\"http://www.musicxml.org/dtds/partwise.dtd\">" << std::endl;

    // Sheet music format
    open("score-partwise", "version=\"3.0\"");
    open("part-list");
    open("score-part", "id=\"P1\"");
    print("part-name", "");
    close();
    close();
    open("part", "id=\"P1\"");

    std::vector<Measure> measures = sheet.getAllMeasures();
    for (unsigned int i = 0; i < measures.size(); i++)
    {
        addMeasure();
        std::vector<Note> notes = measures[i].getAllNotes();
        for (unsigned int j = 0; j < notes.size(); j++)
        {
            addNote(notes[j].getPitch(), notes[j].getstart(),
                    notes[j].getDuration(), measures[i].getBeat());
        }
        close();
    }

    while (tags.size() > 0)
        close();
    output.close();

    return true;
}

void SheetMusicConverter::addNote(int pitch, float start, float duration, float beat_duration, int divisions)
{
    open("note");
    if (pitch == 0)
    {
        for (unsigned int i = 0; i < tags.size(); i++)
            output << "\t";
        std::cout << "<rest/>" << std::endl;
    }
    else
    {
        open("pitch");
        std::string letter;
        std::string alter;
        Converter::getNote(pitch, letter, alter);
        print("step", letter);
        print("alter", alter);
        std::ostringstream octave;
        octave << (pitch / 12) - 1;
        print("octave", octave.str());
        close();
    }
    float beats = duration / beat_duration;
    std::ostringstream d;
    d << (int) (beats / divisions + 0.5);
    print("duration", d.str());
    close();
}

void SheetMusicConverter::addMeasure(int divisions, int key, int beats, int beat_type, bool treble)
{
    std::ostringstream n;
    n << "number=\"" << measure_number << "\"";
    open("measure", n.str());
    open("attributes");
    std::ostringstream d;
    d << divisions;
    print("divisions", d.str());
    if (last_key != key || last_beats != beats || last_beat_type != beat_type || last_clef != treble)
    {
        last_key = key;
        last_beats = beats;
        last_beat_type = beat_type;
        last_clef = treble;
        open("key");
        std::ostringstream k;
        k << key;
        print("fifths", k.str());
        close();
        open("time");
        std::ostringstream b;
        b << beats;
        print("beats", b.str());
        std::ostringstream t;
        t << beat_type;
        print("beat-type", t.str());
        close();
        open("clef");
        if (treble)
        {
            print("sign", "G");
            print("line", "2");
        }
        close();
    }
    close();
    measure_number++;
}
