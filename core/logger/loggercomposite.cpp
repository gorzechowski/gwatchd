#include "loggercomposite.h"

LoggerComposite::LoggerComposite()
{

}

void LoggerComposite::log(QString content)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->log(content);
    }
}

void LoggerComposite::add(Logger *logger)
{
    this->m_loggers << logger;
}
