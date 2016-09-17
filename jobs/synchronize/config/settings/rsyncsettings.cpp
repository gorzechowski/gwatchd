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

#include "rsyncsettings.h"

RsyncSettings::RsyncSettings(QString source, QStringList excludes, QStringList includes, QStringList targetHosts, QString targetPath, QString targetUser)
{
    this->m_source = source;
    this->m_excludes = excludes;
    this->m_includes = includes;
    this->m_targetHosts = targetHosts;
    this->m_targetPath = targetPath;
    this->m_targetUser = targetUser;
}

QString RsyncSettings::source()
{
    return this->m_source;
}

QStringList RsyncSettings::excludes()
{
    return this->m_excludes;
}

QStringList RsyncSettings::includes()
{
    return this->m_includes;
}

QStringList RsyncSettings::targetHosts()
{
    return this->m_targetHosts;
}

QString RsyncSettings::targetPath()
{
    return this->m_targetPath;
}

QString RsyncSettings::targetUser()
{
    return this->m_targetUser;
}
