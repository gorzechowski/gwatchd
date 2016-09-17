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

#include <QDebug>
#include "command/rsync/rsynccommandpartremoteshell.h"
#include "command/ssh/sshcommandbuilder.h"

RsyncCommandPartRemoteShell::RsyncCommandPartRemoteShell(SshSettings *sshSettings)
{
    this->m_settings = sshSettings;
}

QString RsyncCommandPartRemoteShell::build()
{
    QString rsh = "-e \"%1\"";
    SshCommandBuilder builder(this->m_settings);
    QStringList commands = builder.build();

    if(commands.count() == 0) {
        return NULL;
    }

    return rsh.arg(commands.at(0));
}
