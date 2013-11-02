#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser parse;

    return a.exec();
}
