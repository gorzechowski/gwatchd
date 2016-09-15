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

#ifndef SSHCOMMANDBUILDER_H
#define SSHCOMMANDBUILDER_H

#include <QFileInfo>

#include "command/commandbuilder.h"
#include "command/commandpart.h"
#include "config/commandconfig.h"
#include "../../core/job/job.h"

class SshCommandBuilder : public CommandBuilder
{
public:
    SshCommandBuilder(QString entry, CommandConfig *config);

    QStringList build();

protected:
    QList<CommandPart*> m_parts;
    QString m_entry;
    CommandConfig *m_config;
};

#endif // SSHCOMMANDBUILDER_H
