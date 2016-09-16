#ifndef RSYNCSETTINGS_H
#define RSYNCSETTINGS_H

#include <QString>
#include <QStringList>

class RsyncSettings
{
public:
    RsyncSettings(
        QString source,
        QStringList excludes,
        QStringList includes,
        QStringList targetHosts,
        QString targetPath,
        QString targetUser
    );

    QString source();
    QStringList excludes();
    QStringList includes();
    QStringList targetHosts();
    QString targetPath();
    QString targetUser();

protected:
    QString m_source;
    QStringList m_excludes;
    QStringList m_includes;
    QStringList m_targetHosts;
    QString m_targetPath;
    QString m_targetUser;
};

#endif // RSYNCSETTINGS_H
