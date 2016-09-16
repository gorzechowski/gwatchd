#include "sshconfig.h"

SshConfig::SshConfig(Config *config): JobConfig(config)
{
    this->m_config = config;
}

QStringList SshConfig::sshHosts()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("hosts").toArray());
}

QStringList SshConfig::sshHosts(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("hosts");

    if(!value.isArray()) {
        return this->sshHosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString SshConfig::sshUser()
{
    return this->m_config->value("ssh").toObject().value("user").toString();
}

QString SshConfig::sshUser(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->sshUser();
    }

    return value;
}

QString SshConfig::sshIdentityFile()
{
    return this->m_config->value("ssh").toObject().value("identityFile").toString();
}

QString SshConfig::sshIdentityFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("identityFile").toString();

    if(value.isEmpty()) {
        return this->sshIdentityFile();
    }

    return value;
}

QString SshConfig::sshConfigFile()
{
    return this->m_config->value("ssh").toObject().value("configFile").toString();
}

QString SshConfig::sshConfigFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("configFile").toString();

    if(value.isEmpty()) {
        return this->sshConfigFile();
    }

    return value;
}

int SshConfig::sshPort()
{
    return this->m_config->value("ssh").toObject().value("port").toInt(0);
}

int SshConfig::sshPort(QString entry)
{
    int value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("port").toInt(-1);

    if(value == -1) {
        return this->sshPort();
    }

    return value;
}

QStringList SshConfig::sshOptions()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("options").toArray());
}

QStringList SshConfig::sshOptions(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("options");

    if(!value.isArray()) {
        return this->sshOptions();
    }

    return this->m_config->toStringList(value.toArray());
}
