#ifndef SYNCHRONIZECONFIG_H
#define SYNCHRONIZECONFIG_H

#include <QObject>

#include "../../../core/config/config.h"

class SynchronizeConfig : public QObject
{
    Q_OBJECT
public:
    SynchronizeConfig(Config *config, QObject *parent = 0);

    QJsonValue value(QString key);

    QStringList entries();
    QStringList excludes(QString entry);
    QStringList includes(QString entry);
    QStringList targetHosts();
    QStringList targetHosts(QString entry);
    QString targetPath();
    QString targetPath(QString entry);
    QString targetUser();
    QString targetUser(QString entry);
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

#endif // SYNCHRONIZECONFIG_H
