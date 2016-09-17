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

#include "rsyncsettingsfactory.h"

RsyncSettingsFactory::RsyncSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

RsyncSettings RsyncSettingsFactory::create(Entry entry, Config *config)
{
    RsyncSettingsFactory factory("dirs", config);

    return factory.create(entry);
}

RsyncSettings RsyncSettingsFactory::create(QString entry)
{
    return RsyncSettings(
        entry,
        this->excludes(entry),
        this->includes(entry),
        this->targetHosts(entry),
        this->targetPath(entry),
        this->targetUser(entry)
    );
}

QStringList RsyncSettingsFactory::excludes(QString entry)
{
    return this->m_config->toStringList(this->m_config->value(this->m_context).toObject().value(entry).toObject().value("excludes").toArray());
}

QStringList RsyncSettingsFactory::includes(QString entry)
{
    return this->m_config->toStringList(this->m_config->value(this->m_context).toObject().value(entry).toObject().value("includes").toArray());
}

QStringList RsyncSettingsFactory::targetHosts()
{
    return this->m_config->toStringList(this->m_config->value("target").toObject().value("hosts").toArray());
}

QStringList RsyncSettingsFactory::targetHosts(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("target").toObject().value("hosts");

    if(!value.isArray()) {
        return this->targetHosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString RsyncSettingsFactory::targetPath()
{
    return this->m_config->value("target").toObject().value("path").toString();
}

QString RsyncSettingsFactory::targetPath(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("target").toObject().value("path").toString();

    if(value.isEmpty()) {
        return this->targetPath();
    }

    return value;
}

QString RsyncSettingsFactory::targetUser()
{
    return this->m_config->value("target").toObject().value("user").toString();
}

QString RsyncSettingsFactory::targetUser(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("target").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->targetUser();
    }

    return value;
}
