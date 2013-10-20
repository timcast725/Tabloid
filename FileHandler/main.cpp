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
        //Testing ChangeDirectory----------
        cout << "Input directory to navigate to: ";
        string next_directory;
        cin >> next_directory;
        QString next_directory_q = QString::fromStdString(next_directory);

        test_handler.ChangeDirectory(next_directory_q);
        cout << "Current open directory: " <<
            test_handler.CurrentDirectory().currentPath().toStdString()
            << "\n";
        print_directory_contents(test_handler.CurrentDirectory());
        //--------------------------------
    }

    return a.exec();
}
