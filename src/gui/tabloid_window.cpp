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

#include "converter.h"
#include "parser.h"
#include "sheet_music.h"
#include "tabloid_window.h"

#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QPushButton>

TabloidWindow::TabloidWindow(int w, int h, QWidget *parent) : QWidget(parent)
{
    width = w;
    height = h;
    resize(width, height);
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width() - width) / 2, (desktop->height() - height) / 2);
    setWindowTitle("Tabloid");

    QPushButton *open_button = new QPushButton("Open", this);
    open_button->setGeometry((width - 50) / 2, height - 80, 50, 25);
    connect(open_button, SIGNAL(clicked()), this, SLOT(clickOpen()));

    QPushButton *transcribe_button = new QPushButton("Transcribe", this);
    transcribe_button->setGeometry((width - 100) / 2, height - 40, 100, 25);
    connect(transcribe_button, SIGNAL(clicked()), this, SLOT(clickTranscribe()));
}

void TabloidWindow::clickOpen()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Wave Files (*.wav)"));
    std::cout << file_name.toUtf8().constData() << std::endl;
}

void TabloidWindow::clickTranscribe()
{
    if (file_name.isEmpty())
    {
        std::cerr << "Empty file name" << std::endl;
        return;
    }
    SheetMusic music;
    Parser parse;
    if (!parse.Parse((char_t *) file_name.toUtf8().constData(), 4, music))
    {
        std::cerr << "Failed to parse" << std::endl;
        return;
    }
    Converter convert;
    file_name.chop(4);
    file_name.append(".xml");
    convert.Convert(file_name.toStdString(), music);
}