#ifndef ENTRYDESCRIPTOR_H
#define ENTRYDESCRIPTOR_H

#include <QString>

#include "config/commandconfig.h"
#include "../../core/job/job.h"

class EntryDescriptor
{
public:
    EntryDescriptor(
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
    );

    static EntryDescriptor create(Entry entry, CommandConfig *config);
    static EntryDescriptor create(Predefine predefine, CommandConfig *config);

    QString entry();
    bool remote();
    QString exec();
    QString fileMask();
    QStringList sshHosts();
    QString sshUser();
    QString sshIdentityFile();
    QString sshConfigFile();
    int sshPort();
    QStringList sshOptions();

protected:
    QString m_entry;
    bool m_remote;
    QString m_exec;
    QString m_fileMask;
    QStringList m_sshHosts;
    QString m_sshUser;
    QString m_sshIdentityFile;
    QString m_sshConfigFile;
    int m_sshPort;
    QStringList m_sshOptions;
};

#endif // ENTRYDESCRIPTOR_H
