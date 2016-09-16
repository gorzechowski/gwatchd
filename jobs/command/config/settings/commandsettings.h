#ifndef COMMANDSETTINGS_H
#define COMMANDSETTINGS_H

#include <QString>

class CommandSettings
{
public:
    CommandSettings(bool remote, QString exec, QString fileMask);

    bool remote();
    QString exec();
    QString fileMask();

protected:
    bool m_remote;
    QString m_exec;
    QString m_fileMask;
};

#endif // COMMANDSETTINGS_H
