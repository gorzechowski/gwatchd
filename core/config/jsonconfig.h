#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QObject>
#include <QVariant>
#include <QStringList>

#include "config/config.h"

class JsonConfig : public Config
{
    Q_OBJECT
public:
    JsonConfig(QString filePath, QObject *parent = 0);

    QJsonValue value(QString key);

protected:
    QJsonObject m_main;
};

#endif // JSONCONFIG_H
