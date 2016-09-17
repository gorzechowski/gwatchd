#include "hookssettings.h"

HooksSettings::HooksSettings(QList<HookDescriptor> finishedHooks, QList<HookDescriptor> failedHooks)
{
    this->m_finishedHooks = finishedHooks;
    this->m_failedHooks = failedHooks;
}

QList<HookDescriptor> HooksSettings::finishedHooks()
{
    return this->m_finishedHooks;
}

QList<HookDescriptor> HooksSettings::failedHooks()
{
    return this->m_failedHooks;
}
