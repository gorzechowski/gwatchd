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

#include "commandsettingsfactory.h"

CommandSettingsFactory::CommandSettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

CommandSettings CommandSettingsFactory::create(Entry entry, Config *config)
{
    CommandSettingsFactory factory("entries", config);

    return factory.create(entry);
}

CommandSettings CommandSettingsFactory::create(Predefine predefine, Config *config)
{
    CommandSettingsFactory factory("predefines", config);

    return factory.create(predefine);
}

CommandSettings CommandSettingsFactory::create(QString entry)
{
    return CommandSettings(
        this->remote(entry),
        this->exec(entry),
        this->workingDir(entry)
    );
}

bool CommandSettingsFactory::remote(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("remote").toBool(false);
}

QString CommandSettingsFactory::exec(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("exec").toString();
}

QString CommandSettingsFactory::workingDir(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("workingDir").toString();
}
