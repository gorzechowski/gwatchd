#include "applicationconfig.h"

ApplicationConfig::ApplicationConfig(Config *config, QObject *parent) : QObject(parent)
{
    this->m_config = config;
}

QFileInfo ApplicationConfig::fileInfo()
{
    return this->m_config->fileInfo();
}

QString ApplicationConfig::logsDirPath()
{
    return this->m_config->value("log").toObject().value("dirPath").toString("logs");
}

int ApplicationConfig::logsMaxFileSize()
{
    return this->m_config->value("log").toObject().value("maxFileSize").toInt(5);
}

QString ApplicationConfig::socketAddress()
{
    return this->m_config->value("socket").toObject().value("address").toString("");
}

int ApplicationConfig::socketPort()
{
    return this->m_config->value("socket").toObject().value("port").toInt(0);
}
