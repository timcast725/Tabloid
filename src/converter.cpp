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

#include "converter.h"
#include "note.h"

#include <iostream>
#include <sstream>


Converter::Converter()
{
    measure_number_ = 1;
}

bool Converter::Convert(const std::string &name, const SheetMusic &sheet)
{
    output_.open(name, ios::out | ios::trunc);

    // MusicXML header
    output_ << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    output_ << std::endl;
    output_ << "<!DOCTYPE score-partwise PUBLIC" << std::endl;
    output_ << "\"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << std::endl;
    output_ << "\"http://www.musicxml.org/dtds/partwise.dtd\">" << std::endl;

    // Sheet music format
    Open("score-partwise", "version=\"3.0\"");
    Open("part-list");
    Open("score-part", "id=\"P1\"");
    Print("part-name", "");
    Close();
    Close();
    Open("part", "id=\"P1\"");

    std::vector<Measure> measures = sheet.GetAllMeasures();
    for (int i = 0; i < measures.size(); i++)
    {
        AddMeasure();
        std::vector<Note> notes = measures[i].GetAllNotes();
        for (int j = 0; j < notes.size(); j++)
        {
            AddNote(notes[j].GetPitch(), notes[j].GetStart(),
                    notes[j].GetDuration(), measures[i].GetBeat());
        }
        Close();
    }

    while (tags_.size() > 0)
        Close();
    output_.close();

    return true;
}

void Converter::Open(std::string tag, std::string option)
{
    for (int i = 0; i < tags_.size(); i++)
        output_ << "\t";
    if (option == "")
        output_ << "<" << tag << ">" << std::endl;
    else
        output_ << "<" << tag << " " << option << ">" << std::endl;
    tags_.push_back(tag);
}

void Converter::Print(std::string tag, std::string content)
{
    for (int i = 0; i < tags_.size(); i++)
        output_ << "\t";
    output_ << "<" << tag << ">" << content << "</" << tag << ">" << std::endl;
}

void Converter::Close()
{
    for (int i = 1; i < tags_.size(); i++)
        output_ << "\t";
    output_ << "</" << tags_.back() << ">" << std::endl;
    tags_.pop_back();
}

void Converter::AddNote(int pitch, float start, float duration, float beat_duration, int divisions)
{
    Open("note");
    if (pitch == 0)
    {
        for (int i = 0; i < tags_.size(); i++)
            output_ << "\t";
        std::cout << "<rest/>" << std::endl;
    }
    else
    {
        Open("pitch");
        switch (pitch % 12)
        {
            case 0:
                Print("step", "C");
                break;
            case 1:
                Print("step", "C");
                Print("alter", "1");
                break;
            case 2:
                Print("step", "D");
                break;
            case 3:
                Print("step", "E");
                Print("alter", "-1");
                break;
            case 4:
                Print("step", "E");
                break;
            case 5:
                Print("step", "F");
                break;
            case 6:
                Print("step", "F");
                Print("alter", "1");
                break;
            case 7:
                Print("step", "G");
                break;
            case 8:
                Print("step", "A");
                Print("alter", "-1");
                break;
            case 9:
                Print("step", "A");
                break;
            case 10:
                Print("step", "B");
                Print("alter", "-1");
                break;
            case 11:
                Print("step", "B");
                break;
        }
        std::ostringstream octave;
        octave << (pitch / 12) - 1;
        Print("octave", octave.str());
        Close();
    }
    float beats = duration / beat_duration;
    std::ostringstream d;
    d << (int) (beats / divisions + 0.5);
    Print("duration", d.str());
    Close();
}

void Converter::AddMeasure(int divisions, int key, int beats, int beat_type, bool treble)
{
    std::ostringstream n;
    n << "number=\"" << measure_number_ << "\"";
    Open("measure", n.str());
    Open("attributes");
    std::ostringstream d;
    d << divisions;
    Print("divisions", d.str());
    Open("key");
    std::ostringstream k;
    k << key;
    Print("fifths", k.str());
    Close();
    Open("time");
    std::ostringstream b;
    b << beats;
    Print("beats", b.str());
    std::ostringstream t;
    t << beat_type;
    Print("beat-type", t.str());
    Close();
    Open("clef");
    if (treble)
    {
        Print("sign", "G");
        Print("line", "2");
    }
    Close();
    Close();
    measure_number_++;
}