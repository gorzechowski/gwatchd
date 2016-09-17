#ifndef HOOKSSETTINGSFACTORY_H
#define HOOKSSETTINGSFACTORY_H

#include <QString>

#include "../hookssettings.h"
#include "../hookdescriptor.h"
#include "config/config.h"
#include "job/job.h"

class HooksSettingsFactory
{
public:
    HooksSettingsFactory(QString context, Config *config);

    static HooksSettings create(Entry entry, Config *config);

    HooksSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    QList<HookDescriptor> finishedHooks(QString entry);
    QList<HookDescriptor> failedHooks(QString entry);
    QList<HookDescriptor> hooks(QString type, QString entry);
};

#endif // HOOKSSETTINGSFACTORY_H
