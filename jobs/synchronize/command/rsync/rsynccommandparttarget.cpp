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

#include "command/rsync/rsynccommandparttarget.h"

RsyncCommandPartTarget::RsyncCommandPartTarget(RsyncSettings *settings)
{
    this->m_settings = settings;
}

QString RsyncCommandPartTarget::build(QString host)
{
    this->m_host = host;

    return this->build();
}

QString RsyncCommandPartTarget::build()
{
    QString target = "%1@%2:%3";
    QFileInfo entry(this->m_settings->source());

    QString user = this->m_settings->targetUser();

    if(user.isEmpty()) {
        target.remove("@");
    }

    QString targetPath = this->m_settings->targetPath();

    if(entry.isDir() && !targetPath.endsWith("/")) {
        targetPath.append("/");
    } else if(entry.isFile() && targetPath.endsWith("/")) {
        targetPath = targetPath.left(targetPath.length() - 1);
    }

    return target.arg(user, this->m_host, targetPath);
}
