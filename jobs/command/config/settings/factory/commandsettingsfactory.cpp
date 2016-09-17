#include "commandsettingsfactory.h"

CommandSettingsFactory::CommandSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

CommandSettings CommandSettingsFactory::create(Entry entry, Config *config)
{
    CommandSettingsFactory factory("dirs", config);

    return factory.create(entry);
}

CommandSettings CommandSettingsFactory::create(Predefine predefine, Config *config)
{
    CommandSettingsFactory factory("predefines", config);

    return factory.create(predefine);
}

CommandSettings CommandSettingsFactory::create(QString entry)
{
    return CommandSettings(
        this->remote(entry),
        this->exec(entry),
        this->fileMask(entry)
    );
}

bool CommandSettingsFactory::remote(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("remote").toBool(false);
}

QString CommandSettingsFactory::exec(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("exec").toString();
}

QString CommandSettingsFactory::fileMask(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("fileMask").toString();
}
