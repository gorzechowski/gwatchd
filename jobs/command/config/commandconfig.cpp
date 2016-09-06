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

#include "commandconfig.h"

CommandConfig::CommandConfig(Config *config, QObject *parent) : QObject(parent)
{
    this->m_config = config;
}

QJsonValue CommandConfig::value(QString key)
{
    return this->m_config->value(key);
}

QStringList CommandConfig::entries()
{
    return this->m_config->value("dirs").toObject().keys();
}

bool CommandConfig::remote(QString entry)
{
    return this->m_config->value("dirs").toObject().value(entry).toObject().value("remote").toBool(false);
}

QString CommandConfig::exec(QString entry)
{
    return this->m_config->value("dirs").toObject().value(entry).toObject().value("exec").toString();
}

QString CommandConfig::fileMask(QString entry)
{
    return this->m_config->value("dirs").toObject().value(entry).toObject().value("fileMask").toString();
}

QStringList CommandConfig::sshHosts()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("hosts").toArray());
}

QStringList CommandConfig::sshHosts(QString entry)
{
    QJsonValue value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("hosts");

    if(!value.isArray()) {
        return this->sshHosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString CommandConfig::sshUser()
{
    return this->m_config->value("ssh").toObject().value("user").toString();
}

QString CommandConfig::sshUser(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->sshUser();
    }

    return value;
}

QString CommandConfig::sshIdentityFile()
{
    return this->m_config->value("ssh").toObject().value("identityFile").toString();
}

QString CommandConfig::sshIdentityFile(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("identityFile").toString();

    if(value.isEmpty()) {
        return this->sshIdentityFile();
    }

    return value;
}

QString CommandConfig::sshConfigFile()
{
    return this->m_config->value("ssh").toObject().value("configFile").toString();
}

QString CommandConfig::sshConfigFile(QString entry)
{
    QString value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("configFile").toString();

    if(value.isEmpty()) {
        return this->sshConfigFile();
    }

    return value;
}

int CommandConfig::sshPort()
{
    return this->m_config->value("ssh").toObject().value("port").toInt(0);
}

int CommandConfig::sshPort(QString entry)
{
    int value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("port").toInt(-1);

    if(value == -1) {
        return this->sshPort();
    }

    return value;
}

QStringList CommandConfig::sshOptions()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("options").toArray());
}

QStringList CommandConfig::sshOptions(QString entry)
{
    QJsonValue value = this->m_config->value("dirs").toObject().value(entry).toObject().value("ssh").toObject().value("options");

    if(!value.isArray()) {
        return this->sshOptions();
    }

    return this->m_config->toStringList(value.toArray());
}
