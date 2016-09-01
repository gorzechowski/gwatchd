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

#include "command/rsync/rsynccommandpartremoteshell.h"

RsyncCommandPartRemoteShell::RsyncCommandPartRemoteShell(QString dir, SynchronizeConfig *config)
{
    this->m_dir = dir;
    this->m_config = config;
}

QString RsyncCommandPartRemoteShell::build()
{
    QString rsh = "-e \"ssh %1\"";
    QStringList args = this->getArgs();

    if(args.empty()) {
        return NULL;
    }

    return rsh.arg(args.join(" "));
}

QStringList RsyncCommandPartRemoteShell::getArgs()
{
    QStringList args;

    QString keyFile = this->m_config->sshIdentityFile(this->m_dir);

    if(!keyFile.isEmpty()) {
        args << "-i " + keyFile;
    }

    QString configFile = this->m_config->sshConfigFile(this->m_dir);

    if(!configFile.isEmpty()) {
        args << "-F " + configFile;
    }

    int port = this->m_config->sshPort(this->m_dir);

    if(port > 0) {
        args << "-p " + QString::number(port);
    }

    QStringList options = this->m_config->sshOptions(this->m_dir);

    if(!options.empty()) {
        foreach(QString option, options) {
            args << "-o " + option;
        }
    }

    return args;
}
