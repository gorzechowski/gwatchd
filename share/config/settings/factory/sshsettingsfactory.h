#ifndef SSHSETTINGSFACTORY_H
#define SSHSETTINGSFACTORY_H

#include <QString>

#include "../../../../core/job/job.h"
#include "../../../../core/config/config.h"
#include "../sshsettings.h"

class SshSettingsFactory
{
public:
    SshSettingsFactory(QString context, Config *config);

    static SshSettings create(Entry entry, Config *config);
    static SshSettings create(Predefine predefine, Config *config);

    SshSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    QStringList hosts();
    QStringList hosts(QString entry);
    QString user();
    QString user(QString entry);
    QString identityFile();
    QString identityFile(QString entry);
    QString configFile();
    QString configFile(QString entry);
    int port();
    int port(QString entry);
    QStringList options();
    QStringList options(QString entry);
};

#endif // SSHSETTINGSFACTORY_H
