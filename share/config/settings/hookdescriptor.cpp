#include "hookdescriptor.h"

HookDescriptor::HookDescriptor(QString jobName, Predefine predefine)
{
    this->m_jobName = jobName;
    this->m_predefine = predefine;
}

QString HookDescriptor::jobName()
{
    return this->m_jobName;
}

Predefine HookDescriptor::predefine()
{
    return this->m_predefine;
}
