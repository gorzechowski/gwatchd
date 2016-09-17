/*
 * Copyright (C) 2015 - 2016 Gracjan Orzechowski
 *
 * This file is part of GWatchD
 *
 * GWatchD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GWatchD; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

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
