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

#include "command/rsync/rsynccommandpartincludes.h"

RsyncCommandPartIncludes::RsyncCommandPartIncludes(QString dir, Config *config)
{
    this->m_config = config;
    this->m_dir = dir;
}

QString RsyncCommandPartIncludes::build()
{
    QStringList list = this->m_config->listValue(QString("dirs.%1.includes").arg(this->m_dir));
    QStringList includes;

    foreach(QString file, list) {
        includes.append("--include=\""+file+"\"");
    }

    return includes.join(" ");
}
