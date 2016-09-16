#ifndef HOOKSCONFIG_H
#define HOOKSCONFIG_H

#include <QList>
#include <QPair>
#include <QString>

#include "config/config.h"
#include "../share/config/jobconfig.h"

class HooksConfig: public JobConfig
{
public:
    HooksConfig(Config *config);

    QList<QPair<QString, QString> > finishedHooks(QString entry);
    QList<QPair<QString, QString> > failedHooks(QString entry);
    QList<QPair<QString, QString> > hooks(QString type, QString entry);
};

#endif // HOOKSCONFIG_H
