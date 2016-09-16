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

CommandConfig::CommandConfig(Config *config, QObject *parent) : QObject(parent), BaseJobConfig(config), SshConfig(config)
{

}

bool CommandConfig::remote(QString entry)
{
    return this->m_config->value(this->context()).toObject().value(entry).toObject().value("remote").toBool(false);
}

QString CommandConfig::exec(QString entry)
{
    return this->m_config->value(this->context()).toObject().value(entry).toObject().value("exec").toString();
}

QString CommandConfig::fileMask(QString entry)
{
    return this->m_config->value(this->context()).toObject().value(entry).toObject().value("fileMask").toString();
}
