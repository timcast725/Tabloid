// Copyright 2014 Timothy Castiglia

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

#include "guitar_tab_converter.h"
#include "converter.h"
#include "note.h"

#include <iostream>
#include <sstream>

GuitarTabConverter::GuitarTabConverter()
{
    std::pair<std::string,std::string> String1("E","2");
    std::pair<std::string,std::string> String2("A","2");
    std::pair<std::string,std::string> String3("D","3");
    std::pair<std::string,std::string> String4("G","3");
    std::pair<std::string,std::string> String5("B","3");
    std::pair<std::string,std::string> String6("E","4");
    strings.push_back(String1);
    strings.push_back(String2);
    strings.push_back(String3);
    strings.push_back(String4);
    strings.push_back(String5);
    strings.push_back(String6);
}

GuitarTabConverter::GuitarTabConverter(std::vector<std::pair<std::string,std::string> >& input)
{
    strings = input;
}

bool GuitarTabConverter::convert(const std::string &name, const SheetMusic &sheet)
{
    output.open(name.c_str(), ios::out | ios::trunc);

    // MusicXML header
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    output << std::endl;
    output << "<!DOCTYPE score-partwise PUBLIC" << std::endl;
    output << "\"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << std::endl;
    output << "\"http://www.musicxml.org/dtds/partwise.dtd\">" << std::endl;

    //Sheet music format
    open("score-partwise", "version=\"3.0\"");
    open("part-list");
    open("score-part", "id=\"P1\"");
    print("part-name", "");
    close();
    close();
    open("part", "id=\"P1\"");

    //Guitar Tuning
    open("attributes");
    output << "<divisions>2</divisions>" << std::endl;
    open("key", "print-object=\"no\">");
    output << "<fifths>0</fifths>" << std::endl;
    output << "<mode>major</mode>" << std::endl;
    close();
    open("clef");
    output << "<sign>TAB</sign>" << std::endl;
    output << "<line>5</line>" << std::endl;
    close();
    open("staff-details");
    output << "<staff-lines>" << strings.size() << "</staff-lines>" << std::endl;
    for(unsigned int i = 0; i < strings.size(); i++)
    {
        output << "<staff-tuning line=" << (i + 1) << ">" << std::endl;
        output << "<tuning-step>" << strings[i].first << "</tuning-step>" << std::endl;
        output << "<tuning-octave>" << strings[i].second << "</tuning-octave>" << std::endl;
        output << "</staff-tuning>" << std::endl;
    }
    close();
    close();

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

void GuitarTabConverter::addNote(int pitch, float start, float duration, float beat_duration, int divisions)
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
        std::string stringNum = "True";
        std::string fret;
        getNote(pitch, letter, alter, stringNum, fret);
        print("step", letter);
        print("alter", alter);
        std::ostringstream octave;
        octave << (pitch / 12) - 1;
        print("octave", octave.str());
        open("technical");
        open("string");
        output << "<string>" << stringNum << "</fifths>" << std::endl;
        output << "<fret>" << fret << "</fret>" << std::endl;
        close();
        close();
        close();
    }
    float beats = duration / beat_duration;
    std::ostringstream d;
    d << (int) (beats / divisions + 0.5);
    print("duration", d.str());
    close();
}

void GuitarTabConverter::addMeasure(int divisions, int key, int beats, int beat_type, bool treble)
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

void GuitarTabConverter::getNote(int pitch, std::string &letter, std::string &alter,
             std::string &stringNum, std::string& fret)
{
    if(tuning == "Eb")
    {
        pitch++;
    }
    if(tuning == "D")
    {
        pitch += 2;
    }
    if(tuning == "C#")
    {
        pitch += 3;
    }
    if(tuning == "C")
    {
        pitch += 4;
    }
    if(tuning == "B")
    {
        pitch += 5;
    }
    if(tuning == "Bb")
    {
        pitch += 6;
    }
    Converter::getNote(pitch, letter, alter);
    if(stringNum.compare("True") == 0)
    {
        int octave = (pitch / 12) - 1;
        switch(octave)
        {
        case 0:
            if(letter == "E")
            {
                if(alter == "-1")
                {
                    stringNum = "9";
                    fret = "2";
                }
            }
            if(letter == "C")
            {
                if(alter == "1")
                {
                    stringNum = "9";
                    fret = "0";
                }
            }
            if(letter == "D")
            {
                stringNum = "9";
                fret = "1";
            }
            break;
        case 1:
            if(letter == "E")
            {
                if(alter == "-1")
                {
                    stringNum = "7";
                    fret = "4";
                }else{
                    stringNum = "9";
                    fret = "3";
                }
            }
            if(letter == "F")
            {
                if(alter == "1")
                {
                    stringNum = "8";
                    fret = "0";
                }else{
                    stringNum = "9";
                    fret = "4";
                }
            }
            if(letter == "G")
            {
                stringNum = "8";
                fret = "1";
            }
            if(letter == "A")
            {
                if(alter == "-1")
                {
                    stringNum = "8";
                    fret = "2";
                }else{
                    stringNum = "8";
                    fret = "3";
                }
            }
            if(letter == "B")
            {
                if(alter == "-1")
                {
                    stringNum = "8";
                    fret = "4";
                }else{
                    stringNum = "7";
                    fret = "0";
                }
            }
            if(letter == "C")
            {
                if(alter == "1")
                {
                    stringNum = "7";
                    fret = "2";
                }else{
                    stringNum = "7";
                    fret = "1";
                }
            }
            if(letter == "D")
            {
                stringNum = "7";
                fret = "3";
            }
            break;
            case 2:
                if(letter == "E")
                {
                    if(alter == "-1")
                    {
                        stringNum = "4";
                        fret = "1";
                    }else{
                        stringNum = "6";
                        fret = "0";
                    }
                }
                if(letter == "F")
                {
                    if(alter == "1")
                    {
                        stringNum = "6";
                        fret = "2";
                    }else{
                        stringNum = "6";
                        fret = "1";
                    }
                }
                if(letter == "G")
                {
                    stringNum = "6";
                    fret = "3";
                }
                if(letter == "A")
                {
                    if(alter == "-1")
                    {
                        stringNum = "6";
                        fret = "4";
                    }else{
                        stringNum = "5";
                        fret = "0";
                    }
                }
                if(letter == "B")
                {
                    if(alter == "-1")
                    {
                        stringNum = "5";
                        fret = "1";
                    }else{
                        stringNum = "5";
                        fret = "2";
                    }
                }
                if(letter == "C")
                {
                    if(alter == "1")
                    {
                        stringNum = "5";
                        fret = "4";
                    }else{
                        stringNum = "5";
                        fret = "3";
                    }
                }
                if(letter == "D")
                {
                    stringNum = "4";
                    fret = "0";
                }
                break;
            case 3:
                if(letter == "E")
                {
                    if(alter == "-1")
                    {
                        stringNum = "2";
                        fret = "4";
                    }else{
                        stringNum = "4";
                        fret = "2";
                    }
                }
                if(letter == "F")
                {
                    if(alter == "1")
                    {
                        stringNum = "4";
                        fret = "4";
                    }else{
                        stringNum = "4";
                        fret = "3";
                    }
                }
                if(letter == "G")
                {
                    stringNum = "3";
                    fret = "0";
                }
                if(letter == "A")
                {
                    if(alter == "-1")
                    {
                        stringNum = "3";
                        fret = "1";
                    }else{
                        stringNum = "3";
                        fret = "2";
                    }
                }
                if(letter == "B")
                {
                    if(alter == "-1")
                    {
                        stringNum = "3";
                        fret = "3";
                    }else{
                        stringNum = "2";
                        fret = "0";
                    }
                }
                if(letter == "C")
                {
                    if(alter == "1")
                    {
                        stringNum = "2";
                        fret = "2";
                    }else{
                        stringNum = "2";
                        fret = "1";
                    }
                }
                if(letter == "D")
                {
                    stringNum = "2";
                    fret = "3";
                }
                break;
            case 4:
                if(letter == "E")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "11";
                    }else{
                        stringNum = "1";
                        fret = "0";
                    }
                }
                if(letter == "F")
                {
                    if(alter == "1")
                    {
                        stringNum = "1";
                        fret = "2";
                    }else{
                        stringNum = "1";
                        fret = "1";
                    }
                }
                if(letter == "G")
                {
                    stringNum = "1";
                    fret = "3";
                }
                if(letter == "A")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "4";
                    }else{
                        stringNum = "1";
                        fret = "5";
                    }
                }
                if(letter == "B")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "6";
                    }else{
                        stringNum = "1";
                        fret = 7;
                    }
                }
                if(letter == "C")
                {
                    if(alter == "1")
                    {
                        stringNum = "1";
                        fret = "9";
                    }else{
                        stringNum = "1";
                        fret = "8";
                    }
                }
                if(letter == "D")
                {
                    stringNum = "1";
                    fret = "10";
                }
                break;
            case 5:
                if(letter == "E")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "23";
                    }else{
                        stringNum = "1";
                        fret = "12";
                    }
                }
                if(letter == "F")
                {
                    if(alter == "1")
                    {
                        stringNum = "1";
                        fret = "14";
                    }else{
                        stringNum = "1";
                        fret = "13";
                    }
                }
                if(letter == "G")
                {
                    stringNum = "1";
                    fret = "15";
                }
                if(letter == "A")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "16";
                    }else{
                        stringNum = "1";
                        fret = "17";
                    }
                }
                if(letter == "B")
                {
                    if(alter == "-1")
                    {
                        stringNum = "1";
                        fret = "18";
                    }else{
                        stringNum = "1";
                        fret = "19";
                    }
                }
                if(letter == "C")
                {
                    if(alter == "1")
                    {
                        stringNum = "1";
                        fret = "21";
                    }else{
                        stringNum = "1";
                        fret = "20";
                    }
                }
                if(letter == "D")
                {
                    stringNum = "1";
                    fret = "22";
                }
                break;
            case 6:
                if(letter == "E")
                {
                    stringNum = "1";
                    fret = "24";
                }
                break;
        }
    }
}
