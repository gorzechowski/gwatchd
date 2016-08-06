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

void LoggerComposite::debug(QString content)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->debug(content);
    }
}

void LoggerComposite::setDebug(bool isDebug)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->setDebug(isDebug);
    }
}

void LoggerComposite::add(Logger *logger)
{
    this->m_loggers << logger;
}
