#include <stdio.h>

#include "loggermock.h"

LoggerMock::LoggerMock()
{

}

void LoggerMock::log(QString content)
{
    printf("[LoggerMock] %s\n", qPrintable(content));
}
