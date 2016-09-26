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

#include "sshsettingsfactory.h"

SshSettingsFactory::SshSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

SshSettings SshSettingsFactory::create(Entry entry, Config *config)
{
    SshSettingsFactory factory("entries", config);

    return factory.create(entry);
}

SshSettings SshSettingsFactory::create(Predefine predefine, Config *config)
{
    SshSettingsFactory factory("predefines", config);

    return factory.create(predefine);
}

SshSettings SshSettingsFactory::create(QString entry)
{
    return SshSettings(
        this->hosts(entry),
        this->user(entry),
        this->identityFile(entry),
        this->configFile(entry),
        this->port(entry),
        this->options(entry)
    );
}

QStringList SshSettingsFactory::hosts()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("hosts").toArray());
}

QStringList SshSettingsFactory::hosts(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("hosts");

    if(!value.isArray()) {
        return this->hosts();
    }

    return this->m_config->toStringList(value.toArray());
}

QString SshSettingsFactory::user()
{
    return this->m_config->value("ssh").toObject().value("user").toString();
}

QString SshSettingsFactory::user(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("user").toString();

    if(value.isEmpty()) {
        return this->user();
    }

    return value;
}

QString SshSettingsFactory::identityFile()
{
    return this->m_config->value("ssh").toObject().value("identityFile").toString();
}

QString SshSettingsFactory::identityFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("identityFile").toString();

    if(value.isEmpty()) {
        return this->identityFile();
    }

    return value;
}

QString SshSettingsFactory::configFile()
{
    return this->m_config->value("ssh").toObject().value("configFile").toString();
}

QString SshSettingsFactory::configFile(QString entry)
{
    QString value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("configFile").toString();

    if(value.isEmpty()) {
        return this->configFile();
    }

    return value;
}

int SshSettingsFactory::port()
{
    return this->m_config->value("ssh").toObject().value("port").toInt(0);
}

int SshSettingsFactory::port(QString entry)
{
    int value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("port").toInt(-1);

    if(value == -1) {
        return this->port();
    }

    return value;
}

QStringList SshSettingsFactory::options()
{
    return this->m_config->toStringList(this->m_config->value("ssh").toObject().value("options").toArray());
}

QStringList SshSettingsFactory::options(QString entry)
{
    QJsonValue value = this->m_config->value(this->m_context).toObject().value(entry).toObject().value("ssh").toObject().value("options");

    if(!value.isArray()) {
        return this->options();
    }

    return this->m_config->toStringList(value.toArray());
}
