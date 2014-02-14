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
    Open("part-name");
    Print("");
    Close();
    Close();
    Close();
    Open("part", "id=\"P1\"");

    AddMeasure();
    std::vector<Note> notes = sheet.GetAllMeasures()[0].GetAllNotes();
    for (int i = 0; i < notes.size(); i++)
    {

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

void Converter::Print(std::string content)
{
    for (int i = 0; i < tags_.size(); i++)
        output_ << "\t";
    output_ << content << std::endl;
}

void Converter::Close()
{
    for (int i = 1; i < tags_.size(); i++)
        output_ << "\t";
    output_ << "</" << tags_.back() << ">" << std::endl;
    tags_.pop_back();
}

void Converter::AddMeasure(int divisions, int key, int beats, int beat_type, bool treble)
{
    std::ostringstream n;
    n << "number=\"" << measure_number_ << "\"";
    Open("measure", n.str());
    Open("attributes");
    Open("divisions");
    std::ostringstream d;
    d << divisions;
    Print(d.str());
    Close();
    Open("key");
    Open("fifths");
    std::ostringstream k;
    k << key;
    Print(k.str());
    Close();
    Close();
    Open("time");
    Open("beats");
    std::ostringstream b;
    b << beats;
    Print(b.str());
    Close();
    Open("beat-type");
    std::ostringstream t;
    t << beat_type;
    Print(t.str());
    Close();
    Open("clef");
    if (treble)
    {
        Open("sign");
        Print("G");
        Close();
        Open("line");
        Print("2");
        Close();
    }
    Close();
    Close();
}