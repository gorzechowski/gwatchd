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

#ifndef SSHCOMMANDPARTTARGET_H
#define SSHCOMMANDPARTTARGET_H

#include <QFileInfo>

#include "command/commandpart.h"
#include "../../../share/config/settings/sshsettings.h"

class SshCommandPartTarget : public CommandPart
{
public:
    SshCommandPartTarget(SshSettings *settings);

    QString build(QString host);

protected:
    SshSettings *m_settings;
    QString m_host;

    QString build();
};

#endif // SSHCOMMANDPARTTARGET_H
