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

#ifndef SSHSETTINGSFACTORY_H
#define SSHSETTINGSFACTORY_H

#include <QString>

#include "../../../../core/job/job.h"
#include "../../../../core/config/config.h"
#include "../sshsettings.h"

class SshSettingsFactory
{
public:
    SshSettingsFactory(QString context, Config *config);

    static SshSettings create(Entry entry, Config *config);
    static SshSettings create(Predefine predefine, Config *config);

    SshSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    QStringList hosts();
    QStringList hosts(QString entry);
    QString user();
    QString user(QString entry);
    QString identityFile();
    QString identityFile(QString entry);
    QString configFile();
    QString configFile(QString entry);
    int port();
    int port(QString entry);
    QStringList options();
    QStringList options(QString entry);
};

#endif // SSHSETTINGSFACTORY_H
