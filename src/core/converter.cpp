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

#include "converter.h"

void Converter::getNote(int pitch, std::string& letter, std::string& alter)
{
    switch (pitch % 12)
    {
        case 0:
            letter = "C";
            alter = "0";
            break;
        case 1:
            letter = "C";
            alter = "1";
            break;
        case 2:
            letter = "D";
            alter = "0";
            break;
        case 3:
            letter = "E";
            alter = "-1";
            break;
        case 4:
            letter = "E";
            alter = "0";
            break;
        case 5:
            letter = "F";
            alter = "0";
            break;
        case 6:
            letter = "F";
            alter = "1";
            break;
        case 7:
            letter = "G";
            alter = "0";
            break;
        case 8:
            letter = "A";
            alter = "-1";
            break;
        case 9:
            letter = "A";
            alter = "0";
            break;
        case 10:
            letter = "B";
            alter = "-1";
            break;
        case 11:
            letter = "B";
            alter = "0";
            break;
    }
}

void Converter::open(std::string tag, std::string option)
{
    for (unsigned int i = 0; i < tags.size(); i++)
        output << "\t";
    if (option == "")
        output << "<" << tag << ">" << std::endl;
    else
        output << "<" << tag << " " << option << ">" << std::endl;
    tags.push_back(tag);
}

void Converter::print(std::string tag, std::string content)
{
    for (unsigned int i = 0; i < tags.size(); i++)
        output << "\t";
    output << "<" << tag << ">" << content << "</" << tag << ">" << std::endl;
}

void Converter::close()
{
    for (unsigned int i = 1; i < tags.size(); i++)
        output << "\t";
    output << "</" << tags.back() << ">" << std::endl;
    tags.pop_back();
}
