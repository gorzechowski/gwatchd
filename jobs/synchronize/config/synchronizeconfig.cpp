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

#include "synchronizeconfig.h"

SynchronizeConfig::SynchronizeConfig(Config *config, QObject *parent) : QObject(parent)
{
    this->m_config = config;
}

QJsonValue SynchronizeConfig::value(QString key)
{
    return this->m_config->value(key);
}

QStringList SynchronizeConfig::entries()
{
    return this->m_config->value("dirs").toObject().keys();
}

QList<QPair<QString, QString> > SynchronizeConfig::finishedHooks(QString entry)
{
    return this->hooks("finished", entry);
}

QList<QPair<QString, QString> > SynchronizeConfig::failedHooks(QString entry)
{
    return this->hooks("failed", entry);
}

QList<QPair<QString, QString> > SynchronizeConfig::hooks(QString type, QString entry)
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

QString SynchronizeConfig::sshIdentityFile()
{
    return this->m_config->value("ssh").toObject().value("identityFile").toString();
}

QString SynchronizeConfig::sshIdentityFile(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("identityFile").toString();

    if(value.isEmpty()) {
        return this->sshIdentityFile();
    }

    return value;
}

QString SynchronizeConfig::sshConfigFile()
{
    return this->m_config->value("ssh").toObject().value("configFile").toString();
}

QString SynchronizeConfig::sshConfigFile(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("configFile").toString();

    if(value.isEmpty()) {
        return this->sshConfigFile();
    }

    return value;
}

int SynchronizeConfig::sshPort()
{
    return this->m_config->value("ssh").toObject().value("port").toInt(0);
}

int SynchronizeConfig::sshPort(QString entry)
{
    int value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("port").toInt(-1);

    if(value == -1) {
        return this->sshPort();
    }

    return value;
}

QStringList SynchronizeConfig::sshOptions()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("options").toArray());
}

QStringList SynchronizeConfig::sshOptions(QString entry)
{
    QJsonValue value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("options");

    if(!value.isArray()) {
        return this->sshOptions();
    }

    return this->m_config->toStringList(value.toArray());
}
