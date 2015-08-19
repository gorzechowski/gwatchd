#ifndef LOGGERMOCK_H
#define LOGGERMOCK_H

#include "../../core/logger/logger.h"

class LoggerMock : public Logger
{
public:
    LoggerMock();

    void log(QString content);
};

#endif // LOGGERMOCK_H
