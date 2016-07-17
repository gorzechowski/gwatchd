#include <stdio.h>

#include "simplelogger.h"

SimpleLogger::SimpleLogger(QObject *parent): QObject(parent)
{

}

void SimpleLogger::log(QString content)
{
    printf("%s\n", qPrintable(content));
}
