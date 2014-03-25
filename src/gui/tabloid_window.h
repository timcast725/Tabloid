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

#ifndef TABLOID_WINDOW_H
#define TABLOID_WINDOW_H

#include <QString>
#include <QWidget>

class TabloidWindow : public QWidget
{
    Q_OBJECT

public:
    // Tabloid Window currently just contains an open and transcribe button.
    TabloidWindow(int w = 200, int h = 100, QWidget *parent = 0);

public slots:
    // Action for clicking on the open button.
    void clickOpen();
    // Action for clicking the transcribe button.
    void clickTranscribe();
private:
    int width;
    int height;
    QString file_name;
};

#endif