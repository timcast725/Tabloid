
#include "file_handler.h"
#include "wavfile.h"

#include <iostream>

#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QtMultimedia/QAudioOutput>

using namespace std;

//Constructor
FileHandler::FileHandler()
{
    current_directory = QDir::currentPath();
    home_directory = "";
    current_open_file = nullptr;
    //current_open_file_info = 0;
}

QFile* FileHandler::CurrentFile()
{
    //Debugging check
    if (current_open_file == nullptr)
        cout << "No current open file\n";
    return current_open_file;
}

bool FileHandler::OpenFile(QString file_name)
{
    //Check for existence of file
    if (!QFile::exists(file_name))
    {
        cout << "File " << file_name.toStdString() << " does not exist";
        return false;
    }
    //Get the file's type
    QString f_type = QFileInfo(file_name).suffix();
    cout << "File type: " << f_type.toStdString() << "\n";

    //If there is a file currently open, close it first
    if (current_open_file != nullptr)
    {
        CloseFile();
    }

    QString qstring_wav = "wav";
    //Attempt to open file
    cout << "f_type.compare result: " << f_type.compare("wav") << "\n";
    //QString::compare returns 0 if the strings are equal
    if (!f_type.compare("wav"))
    {
        cout << "Recognize f_type as wav\n";
        current_open_file = new WavFile();
        if (!((dynamic_cast<WavFile*>(current_open_file))->open(file_name)))   //appropriate open flags here
        {
            cout << "File " << file_name.toStdString() << " could not be opened";
            return false;
        }
    }
    //Retrieve the file's information
    current_open_file_info = QFileInfo::QFileInfo(file_name);
    return true;
}

bool FileHandler::CloseFile()
{
    //Check that there is a file currently open
    if (current_open_file == nullptr)
    {
        cout << "No file to close\n";
        return false;
    }

    //Close the file
    current_open_file->close();
    //Make sure it is no longer open before nulling current_open_file
    bool success = !current_open_file->isOpen();
    if (success)
    {
        cout << "File successfully closed\n";
        current_open_file = nullptr;
    }

    return success;
}

bool FileHandler::CreateFile(QString file_name, QString file_type)
{
    //Do not try to create the file if it already exists
    if (QFile::exists(file_name))
    {
        cout << "Could not create file, file already exists\n";
        return false;
    }
    QFile file(file_name);
    //Call open function on the QFile to create the file
    file.open(QFile::ReadWrite);
    if (!QFile::exists(file_name))
    {
        cout << "Could not create file " << file_name.toStdString() << "\n";
        return false;
    }

    return true;
}

bool FileHandler::RemoveFile(QString file_name)
{
    //Check for existence of file
    if (!QFile::exists(file_name))
    {
        cout << "File " << file_name.toStdString() << " does not exist";
        return false;
    }
    //Check that it is not the file currently open
    if (current_open_file != nullptr)
        if (file_name == current_open_file->fileName())
        {
            cout << "File " << file_name.toStdString() << " is currently open";
            return false;
        }

    QFile::remove(file_name);
    return true;
}

bool FileHandler::ChangeDirectory(QString directory_path)
{
    if(!current_directory.cd(directory_path))
    {
        cout << "Changing to directory " << directory_path.toStdString() << " failed\n";
        return false;
    }
    else
    {
        QDir::setCurrent(directory_path);
        return true;
    }
}

