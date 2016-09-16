#include "sshsettings.h"

SshSettings::SshSettings(QStringList hosts, QString user, QString identityFile, QString configFile, int port, QStringList options)
{
    this->m_hosts = hosts;
    this->m_user = user;
    this->m_identityFile = identityFile;
    this->m_configFile = configFile;
    this->m_port = port;
    this->m_options = options;
}

QStringList SshSettings::hosts()
{
    return this->m_hosts;
}

QString SshSettings::user()
{
    return this->m_user;
}

QString SshSettings::identityFile()
{
    return this->m_identityFile;
}

QString SshSettings::configFile()
{
    return this->m_configFile;
}

int SshSettings::port()
{
    return this->m_port;
}

QStringList SshSettings::options()
{
    return this->m_options;
}
