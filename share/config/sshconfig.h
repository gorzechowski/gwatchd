#ifndef SSHCONFIG_H
#define SSHCONFIG_H

#include <QString>
#include <QStringList>

#include "config/config.h"
#include "../share/config/jobconfig.h"

class SshConfig: public JobConfig
{
public:
    SshConfig(Config *config);

    QStringList sshHosts();
    QStringList sshHosts(QString entry);
    QString sshUser();
    QString sshUser(QString entry);
    QString sshIdentityFile();
    QString sshIdentityFile(QString entry);
    QString sshConfigFile();
    QString sshConfigFile(QString entry);
    int sshPort();
    int sshPort(QString entry);
    QStringList sshOptions();
    QStringList sshOptions(QString entry);

protected:
    Config *m_config;
};

#endif // SSHCONFIG_H
