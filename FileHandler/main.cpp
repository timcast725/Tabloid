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

void test_change_directory(FileHandler test_handler)
{
    cout << "Input directory to navigate to: ";
    string next_directory;
    cin >> next_directory;
    QString next_directory_q = QString::fromStdString(next_directory);

    test_handler.ChangeDirectory(next_directory_q);
    cout << "Current open directory: " <<
        test_handler.CurrentDirectory().currentPath().toStdString()
        << "\n";
    print_directory_contents(test_handler.CurrentDirectory());
}

void test_file_create(FileHandler test_handler)
{
    cout << "Input new file name: ";
    string new_file_name;
    cin >> new_file_name;
    QString new_file_name_q = QString::fromStdString(new_file_name);
    test_handler.CreateFile(new_file_name_q, "txt");
    cout << "\n";
    print_directory_contents(test_handler.CurrentDirectory());
}

void test_file_remove(FileHandler test_handler)
{
    cout << "Input file name to remove: ";
    string file_name;
    cin >> file_name;
    QString file_name_q = QString::fromStdString(file_name);
    bool success = test_handler.RemoveFile(file_name_q);
    cout << "\n";
    print_directory_contents(test_handler.CurrentDirectory());
}

void test_file_open(FileHandler test_handler)
{
    cout << "Input file name to open: ";
    string file_name;
    cin >> file_name;
    QString file_name_q = QString::fromStdString(file_name);
    bool success = test_handler.OpenFile(file_name_q);
    cout << "\n";
    print_directory_contents(test_handler.CurrentDirectory());
    //QString fname = test_handler.CurrentFile()->fileName();
    if (success && (test_handler.CurrentFile() != nullptr))
        cout << "current_open_file: " << (test_handler.CurrentFile()->fileName()).toStdString();
    cout << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FileHandler test_handler;

    cout << "----------Running FileHandler tests----------\n";

    cout << "Current open directory: " <<
            test_handler.CurrentDirectory().currentPath().toStdString()
         << "\n";
    while(1)
    {
        cout << "Select function to test\n";
        cout << "(a) Change Directory\n";
        cout << "(b) Create New File\n";
        cout << "(c) Remove File\n";
        cout << "(d) Open File\n";

        string choice;
        cin >> choice;
        if (choice == "a")
            test_change_directory(test_handler);
        else if (choice == "b")
            test_file_create(test_handler);
        else if (choice == "c")
            test_file_remove(test_handler);
        else if (choice == "d")
            test_file_open(test_handler);
    }

    return a.exec();
}
