#ifndef COMMANDSETTINGSFACTORY_H
#define COMMANDSETTINGSFACTORY_H

#include <QString>

#include "job/job.h"
#include "config/config.h"
#include "../commandsettings.h"

class CommandSettingsFactory
{
public:
    CommandSettingsFactory(QString context, Config *config);

    static CommandSettings create(Entry entry, Config *config);
    static CommandSettings create(Predefine predefine, Config *config);

    CommandSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    bool remote(QString entry);
    QString exec(QString entry);
    QString fileMask(QString entry);
};

#endif // COMMANDSETTINGSFACTORY_H
