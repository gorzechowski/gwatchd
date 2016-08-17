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

#include <QStringList>
#include <QFileInfo>

#include "command/rsync/rsynccommandbuilder.h"
#include "command/rsync/rsynccommandpartbase.h"
#include "command/rsync/rsynccommandpartincludes.h"
#include "command/rsync/rsynccommandpartexcludes.h"
#include "command/rsync/rsynccommandpartremoteshell.h"
#include "command/rsync/rsynccommandparttarget.h"

RsyncCommandBuilder::RsyncCommandBuilder(QString entry, SynchronizeConfig *config)
{
    this->m_entry = entry;
    this->m_config = config;
}

QStringList RsyncCommandBuilder::build()
{
    QStringList parts, commands, hosts;

    hosts = this->m_config->listValue(
        QString("dirs.%1.target.hosts").arg(this->m_entry),
        this->m_config->listValue("target.hosts")
    );

    QString entry = this->m_entry;

    QFileInfo info(entry);

    if(info.isDir() && !entry.endsWith("/")) {
        entry.append("/");
    }

    foreach(QString host, this->m_config->targetHosts(this->m_dir)) {
        parts.append(RsyncCommandPartBase().build());
        parts.append(RsyncCommandPartIncludes(this->m_entry, this->m_config).build());
        parts.append(RsyncCommandPartExcludes(this->m_entry, this->m_config).build());
        parts.append(entry);
        parts.append(RsyncCommandPartRemoteShell(this->m_entry, this->m_config).build());
        parts.append(RsyncCommandPartTarget(this->m_entry, this->m_config).build(host));

        commands.append(parts.join(" "));
        parts.clear();
    }

    return commands;
}
