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


Converter::Converter()
{
}

bool Converter::Convert(const std::string &name, const SheetMusic &sheet)
{
    std::ofstream output;
    output.open(name, ios::out | ios::trunc);

    // MusicXML header
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    output << std::endl;
    output << "<!DOCTYPE score-partwise PUBLIC" << std::endl;
    output << "\"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << std::endl;
    output << "\"http://www.musicxml.org/dtds/partwise.dtd\">" << std::endl;

    // Sheet music format
    Open("score-partwise", output, "version=\"3.0\"");
    Open("part-list", output);
    Open("score-part", output, "id=\"P1\"");
    Open("part-name", output);
    Print(name, output);
    Close(output);
    Close(output);
    Close(output);

    std::vector<Note> notes = sheet.GetAllMeasures()[0].GetAllNotes();
    for (int i = 0; i < notes.size(); i++)
    {

    }

    while (_tags.size() > 0)
        Close(output);
    output.close();

    return true;
}

void Converter::Open(std::string tag, std::ofstream &output, std::string option)
{
    for (int i = 0; i < _tags.size(); i++)
        output << "\t";
    if (option == "")
        output << "<" << tag << ">" << std::endl;
    else
        output << "<" << tag << " " << option << ">" << std::endl;
    _tags.push_back(tag);
}

void Converter::Print(std::string content, std::ofstream &output)
{
    for (int i = 0; i < _tags.size(); i++)
        output << "\t";
    output << content << std::endl;
}

void Converter::Close(std::ofstream &output)
{
    for (int i = 1; i < _tags.size(); i++)
        output << "\t";
    output << "</" << _tags.back() << ">" << std::endl;
    _tags.pop_back();
}