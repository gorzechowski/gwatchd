#ifndef SSHSETTINGS_H
#define SSHSETTINGS_H

#include <QString>
#include <QStringList>

#include "../../../core/job/job.h"

class SshSettings
{
public:
    SshSettings(
        QStringList hosts,
        QString user,
        QString identityFile,
        QString configFile,
        int port,
        QStringList options
    );

    QStringList hosts();
    QString user();
    QString identityFile();
    QString configFile();
    int port();
    QStringList options();

protected:
    QStringList m_hosts;
    QString m_user;
    QString m_identityFile;
    QString m_configFile;
    int m_port;
    QStringList m_options;
};

#endif // SSHSETTINGS_H
