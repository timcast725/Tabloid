// Copyright 2013 Zev Battad

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


//GenericFile is the superclass from which all
//specific file-type classes inherit.

#ifndef GENERIC_FILE_H
#define GENERIC_FILE_H

class GenericFile
{
public:
    //Abstract functions
    //TODO: Determine what other common functions are needed for all files
    virtual bool Open();
    virtual bool Close();

};

#endif // GENERIC_FILE_H
