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

#include "settingsfactory.h"

SettingsFactory::SettingsFactory(QString context, Config *config)
{
    this->m_context = context;
    this->m_config = config;
}

Settings SettingsFactory::create(Entry entry, Config *config)
{
    SettingsFactory factory("dirs", config);

    return factory.create(entry);
}

Settings SettingsFactory::create(Predefine predefine, Config *config)
{
    SettingsFactory factory("predefines", config);

    return factory.create(predefine);
}

Settings SettingsFactory::create(QString entry)
{
    return Settings(
        this->fileMask(entry)
    );
}

QString SettingsFactory::fileMask(QString entry)
{
    return this->m_config->value(this->m_context).toObject().value(entry).toObject().value("fileMask").toString();
}
