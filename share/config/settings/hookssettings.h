#ifndef HOOKSSETTINGS_H
#define HOOKSSETTINGS_H

#include "hookdescriptor.h"

class HooksSettings
{
public:
    HooksSettings(
        QList<HookDescriptor> finishedHooks,
        QList<HookDescriptor> failedHooks
    );

    QList<HookDescriptor> finishedHooks();
    QList<HookDescriptor> failedHooks();

protected:
    QList<HookDescriptor> m_finishedHooks;
    QList<HookDescriptor> m_failedHooks;
};

#endif // HOOKSSETTINGS_H
