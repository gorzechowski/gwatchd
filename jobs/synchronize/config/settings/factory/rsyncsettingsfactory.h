#ifndef RSYNCSETTINGSFACTORY_H
#define RSYNCSETTINGSFACTORY_H

#include <QString>

#include "job/job.h"
#include "config/config.h"
#include "../rsyncsettings.h"

class RsyncSettingsFactory
{
public:
    RsyncSettingsFactory(QString context, Config *config);

    static RsyncSettings create(Entry entry, Config *config);

    RsyncSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    QString fileMask(QString entry);
    QStringList excludes(QString entry);
    QStringList includes(QString entry);
    QStringList targetHosts();
    QStringList targetHosts(QString entry);
    QString targetPath();
    QString targetPath(QString entry);
    QString targetUser();
    QString targetUser(QString entry);
};

#endif // RSYNCSETTINGSFACTORY_H
