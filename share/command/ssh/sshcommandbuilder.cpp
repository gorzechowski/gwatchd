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

#include "sshcommandbuilder.h"

#include "command/ssh/sshcommandpartbase.h"
#include "command/ssh/sshcommandparttarget.h"

SshCommandBuilder::SshCommandBuilder(SshSettings *settings)
{
    this->m_settings = settings;
}

QStringList SshCommandBuilder::build()
{
    QStringList parts, commands;

    foreach(QString host, this->m_settings->hosts()) {
        parts.append(SshCommandPartBase(this->m_settings).build());
        parts.append(SshCommandPartTarget(this->m_settings).build(host));
        parts.append(QString("\"%1\"").arg(/*this->m_config->exec(this->m_entry)*/""));

        commands.append(parts.join(" "));
        parts.clear();
    }

    return commands;
}
