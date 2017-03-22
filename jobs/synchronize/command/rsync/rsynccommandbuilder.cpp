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

#include "command/rsync/rsynccommandbuilder.h"
#include "command/rsync/rsynccommandpartbase.h"
#include "command/rsync/rsynccommandpartincludes.h"
#include "command/rsync/rsynccommandpartexcludes.h"
#include "command/rsync/rsynccommandpartremoteshell.h"
#include "command/rsync/rsynccommandparttarget.h"

RsyncCommandBuilder::RsyncCommandBuilder(RsyncSettings *rsyncSettings, SshSettings *sshSettings)
{
    this->m_rsyncSettings = rsyncSettings;
    this->m_sshSettings = sshSettings;
}

QStringList RsyncCommandBuilder::build()
{
    QStringList parts, commands;

    QString source = this->m_rsyncSettings->source();

    QFileInfo entry(this->m_rsyncSettings->source());

    if(entry.isDir() && !source.endsWith("/")) {
        source.append("/");
    }

    foreach(QString host, this->m_rsyncSettings->targetHosts()) {
        parts.append(RsyncCommandPartBase().build());
        parts.append(RsyncCommandPartIncludes(this->m_rsyncSettings).build());
        parts.append(RsyncCommandPartExcludes(this->m_rsyncSettings).build());
        parts.append(source);
        parts.append(RsyncCommandPartRemoteShell(this->m_sshSettings).build());
        parts.append(RsyncCommandPartTarget(this->m_rsyncSettings).build(host));

        commands.append(parts.join(" "));
        parts.clear();
    }

    return commands;
}
