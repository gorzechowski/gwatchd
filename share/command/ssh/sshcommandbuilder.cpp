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

#include <QString>
#include <QDebug>

#include "sshcommandbuilder.h"

#include "command/ssh/sshcommandpartbase.h"
#include "command/ssh/sshcommandparttarget.h"

SshCommandBuilder::SshCommandBuilder(SshSettings *sshSettings)
{
    this->m_sshSettings = sshSettings;
    this->m_command = "";
}

SshCommandBuilder::SshCommandBuilder(SshSettings *sshSettings, QString command)
{
    this->m_sshSettings = sshSettings;
    this->m_command = command;
}

QStringList SshCommandBuilder::build()
{
    QStringList parts, commands;
    QStringList hosts = this->m_sshSettings->hosts();

    if(hosts.count() > 0) {
        foreach(QString host, this->m_sshSettings->hosts()) {
            parts.append(SshCommandPartBase(this->m_sshSettings).build());
            parts.append(SshCommandPartTarget(this->m_sshSettings).build(host));

            if(!this->m_command.isEmpty()) {
                parts.append(QString("\"%1\"").arg(this->m_command));
            }

            QString command = parts.join(" ");

            if(command.trimmed() != "ssh") {
                commands.append(command);
            }

            parts.clear();
        }
    } else {
        parts.append(SshCommandPartBase(this->m_sshSettings).build());

        QString command = parts.join(" ");

        if(command.trimmed() != "ssh") {
            commands.append(command);
        }
    }

    return commands;
}
