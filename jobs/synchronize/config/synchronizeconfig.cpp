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

#include <QDebug>

#include "synchronizeconfig.h"

SynchronizeConfig::SynchronizeConfig(Config *config, QObject *parent) : QObject(parent), BaseJobConfig(config), SshConfig(config), HooksConfig(config)
{
    this->m_config = config;
}

QString SynchronizeConfig::fileMask(QString entry)
{
    return this->m_config->value("dirs").toObject().value(entry).toObject().value("fileMask").toString();
}

QStringList SynchronizeConfig::excludes(QString entry)
{
    return this->m_config->toStringList(this->m_config->value("dirs").toObject().value(entry).toObject().value("excludes").toArray());
}

QStringList SynchronizeConfig::includes(QString entry)
{
    return this->m_config->toStringList(this->m_config->value("dirs").toObject().value(entry).toObject().value("includes").toArray());
}

QStringList SynchronizeConfig::targetHosts()
{
    return this->m_config->toStringList(this->m_config->value("target").toObject().value("hosts").toArray());
}

QStringList SynchronizeConfig::targetHosts(QString entry)
{
    QJsonValue value = this->m_config->value("dirs").toObject().value(entry).toObject().value("target").toObject().value("hosts");

    if(!value.isArray()) {
        return this->targetHosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString SynchronizeConfig::targetPath()
{
    return this->m_config->value("target").toObject().value("path").toString();
}

QString SynchronizeConfig::targetPath(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("target").toObject().value("path").toString();

    if(value.isEmpty()) {
        return this->targetPath();
    }

    return value;
}

QString SynchronizeConfig::targetUser()
{
    return this->m_config->value("target").toObject().value("user").toString();
}

QString SynchronizeConfig::targetUser(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("target").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->targetUser();
    }

    return value;
}
