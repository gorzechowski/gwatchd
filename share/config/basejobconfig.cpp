#include "basejobconfig.h"

BaseJobConfig::BaseJobConfig(Config *config): JobConfig(config)
{

}

QJsonValue BaseJobConfig::value(QString key)
{
    return this->m_config->value(key);
}

QStringList BaseJobConfig::entries()
{
    return this->m_config->value(this->m_context).toObject().keys();
}

void BaseJobConfig::setContext(QString context)
{
    this->m_context = context;
}

QString BaseJobConfig::context()
{
    return this->m_context;
}
