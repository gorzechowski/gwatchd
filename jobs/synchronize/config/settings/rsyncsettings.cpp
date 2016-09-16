#include "rsyncsettings.h"

RsyncSettings::RsyncSettings(QString source, QStringList excludes, QStringList includes, QStringList targetHosts, QString targetPath, QString targetUser)
{
    this->m_source = source;
    this->m_excludes = excludes;
    this->m_includes = includes;
    this->m_targetHosts = targetHosts;
    this->m_targetPath = targetPath;
    this->m_targetUser = targetUser;
}

QString RsyncSettings::source()
{
    return this->m_source;
}

QStringList RsyncSettings::excludes()
{
    return this->m_excludes;
}

QStringList RsyncSettings::includes()
{
    return this->m_includes;
}

QStringList RsyncSettings::targetHosts()
{
    return this->m_targetHosts;
}

QString RsyncSettings::targetPath()
{
    return this->m_targetPath;
}

QString RsyncSettings::targetUser()
{
    return this->m_targetUser;
}
