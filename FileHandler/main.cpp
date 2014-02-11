// Copyright Zev Battad 2013

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

#include <QCoreApplication>

#include "file_handler.h"
#include <iostream>

using namespace std;

void print_directory_contents(QDir dir)
{
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                                .arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
}

void test_change_directory(FileHandler * test_handler)
{
    cout << "Input directory to navigate to: ";
    string next_directory;
    cin >> next_directory;
    QString next_directory_q = QString::fromStdString(next_directory);

    test_handler->ChangeDirectory(next_directory_q);
    cout << "Current open directory: " <<
        test_handler->CurrentDirectory().currentPath().toStdString()
        << "\n";
    print_directory_contents(test_handler->CurrentDirectory());
}

void test_file_create(FileHandler * test_handler)
{
    cout << "Input new file name: ";
    string new_file_name;
    cin >> new_file_name;
    QString new_file_name_q = QString::fromStdString(new_file_name);
    test_handler->CreateFile(new_file_name_q, "txt");
    cout << "\n";
    print_directory_contents(test_handler->CurrentDirectory());
}

void test_file_remove(FileHandler * test_handler)
{
    cout << "Input file name to remove: ";
    string file_name;
    cin >> file_name;
    QString file_name_q = QString::fromStdString(file_name);
    bool success = test_handler->RemoveFile(file_name_q);
    cout << "\n";
    print_directory_contents(test_handler->CurrentDirectory());
}

void test_file_open(FileHandler * test_handler)
{
    cout << "Input file name to open: ";
    string file_name;
    cin >> file_name;
    QString file_name_q = QString::fromStdString(file_name);
    bool success = test_handler->OpenFile(file_name_q);
    cout << "\n";
    print_directory_contents(test_handler->CurrentDirectory());
    //QString fname = test_handler->CurrentFile()->fileName();
    if (success && (test_handler->CurrentFile() != nullptr))
        cout << "current_open_file: " << (test_handler->CurrentFile()->fileName()).toStdString();
    cout << "\n";
}

void test_file_close(FileHandler * test_handler)
{
    cout << "Enter any character to close the current open file\n";
    string c;
    cin >> c;
    bool success = test_handler->CloseFile();
    if (success && test_handler->CurrentFile() == nullptr)
        cout << "File successfully closed, no file is currently open\n";
}

void test_read(FileHandler * test_handler)
{
    cout << "Enter number of bytes to read, or enter -1 to read whole file: ";
    string input;
    cin >> input;
    int length = stoi(input);
    int bytes_read = 0;
    char * buffer = new char[999999];
    if (length == -1)
        bytes_read = test_handler->Read(buffer);
    else
        bytes_read = test_handler->Read(buffer, length);
    cout << "Bytes read: " << bytes_read << "\n";
}

void test_write(FileHandler * test_handler)
{
    cout << "To test writing, we will copy raw data from one file to the current open file.\n";
    cout << "Input file to copy from: ";

    QString current_file = test_handler->CurrentFile()->fileName();
    string next_file;
    cin >> next_file;
    QString q_next_file = QString::fromStdString(next_file);

    //Read from the file we want to copy from
    test_handler->OpenFile(q_next_file);
    char * buffer = new char[999999];
    int bytes_read = test_handler->Read(buffer);

    //Write to the file we started from
    test_handler->OpenFile(current_file);
    int bytes_written = test_handler->Write(buffer, bytes_read);
    cout << "Bytes written: " << bytes_written << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FileHandler test_handler;

    while(1)
    {
        cout << "----------Running FileHandler tests----------\n";

        cout << "Current open directory: " <<
                test_handler.CurrentDirectory().currentPath().toStdString()
             << "\n";
        cout << "Current open file: ";
        if (test_handler.CurrentFile() != nullptr)
            cout << test_handler.CurrentFile()->fileName().toStdString();
        else
            cout << "None";
        cout << "\n";

        cout << "Select function to test\n";
        cout << "(a) Change Directory\n";
        cout << "(b) Create New File\n";
        cout << "(c) Remove File\n";
        cout << "(d) Open File\n";
        cout << "(e) Close File\n";
        cout << "(f) Read from file\n";
        cout << "(g) Write to file\n";
        cout << "(q) Quit\n";

        string choice;
        cin >> choice;
        if (choice == "a")
            test_change_directory(&test_handler);
        else if (choice == "b")
            test_file_create(&test_handler);
        else if (choice == "c")
            test_file_remove(&test_handler);
        else if (choice == "d")
            test_file_open(&test_handler);
        else if (choice == "e")
            test_file_close(&test_handler);
        else if (choice == "f")
            test_read(&test_handler);
        else if (choice == "g")
            test_write(&test_handler);
        else if (choice == "q")
            return a.exec();
    }

    return a.exec();
}
