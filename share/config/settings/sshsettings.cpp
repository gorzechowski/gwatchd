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

#include "sshsettings.h"

SshSettings::SshSettings(QStringList hosts, QString user, QString identityFile, QString configFile, int port, QStringList options)
{
    this->m_hosts = hosts;
    this->m_user = user;
    this->m_identityFile = identityFile;
    this->m_configFile = configFile;
    this->m_port = port;
    this->m_options = options;
}

QStringList SshSettings::hosts()
{
    return this->m_hosts;
}

QString SshSettings::user()
{
    return this->m_user;
}

QString SshSettings::identityFile()
{
    return this->m_identityFile;
}

QString SshSettings::configFile()
{
    return this->m_configFile;
}

int SshSettings::port()
{
    return this->m_port;
}

QStringList SshSettings::options()
{
    return this->m_options;
}
