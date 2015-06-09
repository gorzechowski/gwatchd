/*
 * Copyright (C) 2015 Gracjan Orzechowski
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

#ifndef RSYNCCOMMANDPARTTARGET_H
#define RSYNCCOMMANDPARTTARGET_H

#include "command/commandpart.h"
#include "config/config.h"

class RsyncCommandPartTarget : public CommandPart
{
public:
    RsyncCommandPartTarget(QString dir, Config *config);

    QString build(QString host);

protected:
    QString build();

    QString m_dir;
    Config *m_config;
    QString m_host;
};

#endif // RSYNCCOMMANDPARTTARGET_H
