#include "sshsettingsfactory.h"

SshSettingsFactory::SshSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

SshSettings SshSettingsFactory::create(Entry entry, Config *config)
{
    SshSettingsFactory factory("dirs", config);

    return factory.create(entry);
}

SshSettings SshSettingsFactory::create(Predefine predefine, Config *config)
{
    SshSettingsFactory factory("predefine", config);

    return factory.create(predefine);
}

SshSettings SshSettingsFactory::create(QString entry)
{
    return SshSettings(
        this->hosts(entry),
        this->user(entry),
        this->identityFile(entry),
        this->configFile(entry),
        this->port(entry),
        this->options(entry)
    );
}

QStringList SshSettingsFactory::hosts()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("hosts").toArray());
}

QStringList SshSettingsFactory::hosts(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("hosts");

    if(!value.isArray()) {
        return this->hosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString SshSettingsFactory::user()
{
    return this->m_config->value("ssh").toObject().value("user").toString();
}

QString SshSettingsFactory::user(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->user();
    }

    return value;
}

QString SshSettingsFactory::identityFile()
{
    return this->m_config->value("ssh").toObject().value("identityFile").toString();
}

QString SshSettingsFactory::identityFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("identityFile").toString();

    if(value.isEmpty()) {
        return this->identityFile();
    }

    return value;
}

QString SshSettingsFactory::configFile()
{
    return this->m_config->value("ssh").toObject().value("configFile").toString();
}

QString SshSettingsFactory::configFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("configFile").toString();

    if(value.isEmpty()) {
        return this->configFile();
    }

    return value;
}

int SshSettingsFactory::port()
{
    return this->m_config->value("ssh").toObject().value("port").toInt(0);
}

int SshSettingsFactory::port(QString entry)
{
    int value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("port").toInt(-1);

    if(value == -1) {
        return this->port();
    }

    return value;
}

QStringList SshSettingsFactory::options()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("options").toArray());
}

QStringList SshSettingsFactory::options(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("options");

    if(!value.isArray()) {
        return this->options();
    }

    return this->m_config->toStringList(value.toArray());
}
