#ifndef LOGGERCOMPOSITE_H
#define LOGGERCOMPOSITE_H

#include <QObject>
#include <QList>

#include "logger/logger.h"

class LoggerComposite: public QObject, public Logger
{
public:
    LoggerComposite();

    void log(QString content);

    void add(Logger *logger);

protected:
    QList<Logger*> m_loggers;
};

#endif // LOGGERCOMPOSITE_H
