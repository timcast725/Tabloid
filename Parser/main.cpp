#include <iostream>
#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser parse;
    parse.Parse("Bb.wav");

    parse.Parse("sdd_test.wav");

    return a.exec();
}
