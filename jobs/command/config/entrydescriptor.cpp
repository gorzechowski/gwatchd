#include "entrydescriptor.h"

EntryDescriptor::EntryDescriptor(
    QString entry,
    bool remote,
    QString exec,
    QString fileMask,
    QStringList sshHosts,
    QString sshUser,
    QString sshIdentityFile,
    QString sshConfigFile,
    int sshPort,
    QStringList sshOptions
) {
    this->m_entry = entry;
    this->m_remote = remote;
    this->m_exec = exec;
    this->m_fileMask = fileMask;
    this->m_sshHosts = sshHosts;
    this->m_sshUser = sshUser;
    this->m_sshIdentityFile = sshIdentityFile;
    this->m_sshConfigFile = sshConfigFile;
    this->m_sshPort = sshPort;
    this->m_sshOptions = sshOptions;
}

EntryDescriptor EntryDescriptor::create(Entry entry, CommandConfig *config)
{
    return EntryDescriptor(
        entry,
        config->remote(entry),
        config->exec(entry),
        config->fileMask(entry),
        config->sshHosts(entry),
        config->sshUser(entry),
        config->sshIdentityFile(entry),
        config->sshConfigFile(entry),
        config->sshPort(entry),
        config->sshOptions(entry)
    );
}

EntryDescriptor EntryDescriptor::create(Predefine predefine, CommandConfig *config)
{
    return EntryDescriptor(
        predefine,
        config->remote(predefine),
        config->exec(predefine),
        config->fileMask(predefine),
        config->sshHosts(predefine),
        config->sshUser(predefine),
        config->sshIdentityFile(predefine),
        config->sshConfigFile(predefine),
        config->sshPort(predefine),
        config->sshOptions(predefine)
    );
}

QString EntryDescriptor::entry()
{
    return this->m_entry;
}

bool EntryDescriptor::remote()
{
    return this->m_remote;
}

QString EntryDescriptor::exec()
{
    return this->m_exec;
}

QString EntryDescriptor::fileMask()
{
    return this->m_fileMask;
}

QStringList EntryDescriptor::sshHosts()
{
    return this->m_sshHosts;
}

QString EntryDescriptor::sshUser()
{
    return this->m_sshUser;
}

QString EntryDescriptor::sshIdentityFile()
{
    return this->m_sshIdentityFile;
}

QString EntryDescriptor::sshConfigFile()
{
    return this->m_sshConfigFile;
}

int EntryDescriptor::sshPort()
{
    return this->m_sshPort;
}

QStringList EntryDescriptor::sshOptions()
{
    return this->m_sshOptions;
}
