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

#include "command/ssh/sshcommandpartbase.h"

SshCommandPartBase::SshCommandPartBase(SshSettings *settings)
{
    this->m_settings = settings;
}

QString SshCommandPartBase::build()
{
    return "ssh " + this->getArgs().join(" ");
}

QStringList SshCommandPartBase::getArgs()
{
    QStringList args;

    QString keyFile = this->m_settings->identityFile();

    if(!keyFile.isEmpty()) {
        args << "-i " + keyFile;
    }

    QString configFile = this->m_settings->configFile();

    if(!configFile.isEmpty()) {
        args << "-F " + configFile;
    }

    int port = this->m_settings->port();

    if(port > 0) {
        args << "-p " + QString::number(port);
    }

    QStringList options = this->m_settings->options();

    if(!options.empty()) {
        foreach(QString option, options) {
            args << "-o " + option;
        }
    }

    return args;
}
