#ifndef HOOKDESCRIPTOR_H
#define HOOKDESCRIPTOR_H

#include <QString>

#include "job/job.h"

class HookDescriptor
{
public:
    HookDescriptor(QString jobName, Predefine predefine);

    QString jobName();
    Predefine predefine();

protected:
    QString m_jobName;
    Predefine m_predefine;
};

#endif // HOOKDESCRIPTOR_H
