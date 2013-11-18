#include <iostream>
#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser parse;
    parse.AubioInit("Bb.wav");
    parse.AubioProcess();

    parse.AubioInit("sdd_test.wav");
    parse.AubioProcess();

    return a.exec();
}
