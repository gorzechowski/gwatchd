#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#include <QObject>

#include "logger/logger.h"

class SimpleLogger: public QObject, public Logger
{
public:
    SimpleLogger(QObject *parent = 0);

    void log(QString content);
};

#endif // SIMPLELOGGER_H
