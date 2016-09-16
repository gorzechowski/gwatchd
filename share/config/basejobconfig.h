#ifndef BASEJOBCONFIG_H
#define BASEJOBCONFIG_H

#include <QString>
#include <QStringList>
#include <QJsonValue>

#include "config/config.h"
#include "../share/config/jobconfig.h"

class BaseJobConfig: public JobConfig
{
public:
    BaseJobConfig(Config *config);

    QJsonValue value(QString key);
    QStringList entries();

    void setContext(QString context);
    QString context();
};

#endif // BASEJOBCONFIG_H
