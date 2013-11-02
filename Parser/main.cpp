#include <iostream>
#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser parse;
    std::cout << parse.PitchDetection() << std::endl;

    return a.exec();
}
