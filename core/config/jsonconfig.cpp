#include <QFile>
#include <QDebug>

#include "jsonconfig.h"

JsonConfig::JsonConfig(QString filePath, QObject *parent) : Config(filePath, parent)
{
    QFile file(filePath);

    if(file.exists()) {
        file.open(QIODevice::ReadOnly);

        this->m_main = QJsonDocument::fromJson(file.readAll()).object();
    } else {
        this->m_main = QJsonObject();
    }
}

QJsonValue JsonConfig::value(QString key)
{
    QJsonValue value = this->m_main.value(key);

    return value;
}
