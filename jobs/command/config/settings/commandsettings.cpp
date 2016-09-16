#include "commandsettings.h"

CommandSettings::CommandSettings(bool remote, QString exec, QString fileMask)
{
    this->m_remote = remote;
    this->m_exec = exec;
    this->m_fileMask = fileMask;
}

bool CommandSettings::remote()
{
    return this->m_remote;
}

QString CommandSettings::exec()
{
    return this->m_exec;
}

QString CommandSettings::fileMask()
{
    return this->m_fileMask;
}
