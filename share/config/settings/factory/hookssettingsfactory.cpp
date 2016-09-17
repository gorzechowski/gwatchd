#include "hookssettingsfactory.h"

HooksSettingsFactory::HooksSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

HooksSettings HooksSettingsFactory::create(Entry entry, Config *config)
{
    HooksSettingsFactory factory("dirs", config);

    return factory.create(entry);
}

HooksSettings HooksSettingsFactory::create(QString entry)
{
    return HooksSettings(
        this->finishedHooks(entry),
        this->failedHooks(entry)
    );
}

QList<HookDescriptor> HooksSettingsFactory::finishedHooks(QString entry)
{
    return this->hooks("finished", entry);
}

QList<HookDescriptor> HooksSettingsFactory::failedHooks(QString entry)
{
    return this->hooks("failed", entry);
}

QList<HookDescriptor> HooksSettingsFactory::hooks(QString type, QString entry)
{
    QList<HookDescriptor> hooks;

    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("hooks");

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
        QString jobName = object.keys().first();
        Predefine predefine = Predefine(object.value(jobName).toString());

        hooks << HookDescriptor(jobName, predefine);
    }

    return hooks;
}
