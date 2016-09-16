#include "hooksconfig.h"

HooksConfig::HooksConfig(Config *config): JobConfig(config)
{
    this->m_config = config;
}

QList<QPair<QString, QString> > HooksConfig::finishedHooks(QString entry)
{
    return this->hooks("finished", entry);
}

QList<QPair<QString, QString> > HooksConfig::failedHooks(QString entry)
{
    return this->hooks("failed", entry);
}

QList<QPair<QString, QString> > HooksConfig::hooks(QString type, QString entry)
{
    QList<QPair<QString, QString> > hooks;

    QJsonValue value = this->m_config->value("dirs").toObject().value(entry).toObject().value("hooks");

    if(!value.isObject()) {
        return hooks;
    }

    value = value.toObject().value(type);

    if(!value.isArray()) {
        return hooks;
    }

    QJsonArray array = value.toArray();

    foreach(QJsonValue hook, array) {
        if(!hook.isObject()) {
            continue;
        }

        QJsonObject object = hook.toObject();
        QString key = object.keys().first();
        QString value = object.value(key).toString();

        hooks << QPair<QString, QString>(key, value);
    }

    return hooks;
}
