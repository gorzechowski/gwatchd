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

#ifndef SYNCHRONIZECONFIG_H
#define SYNCHRONIZECONFIG_H

#include <QObject>

#include "../../../core/config/config.h"

class SynchronizeConfig : public QObject
{
    Q_OBJECT
public:
    SynchronizeConfig(Config *config, QObject *parent = 0);

    QJsonValue value(QString key);

    QStringList entries();
    QStringList excludes(QString entry);
    QStringList includes(QString entry);
    QStringList targetHosts();
    QStringList targetHosts(QString entry);
    QString targetPath();
    QString targetPath(QString entry);
    QString targetUser();
    QString targetUser(QString entry);
    QString sshIdentityFile();
    QString sshIdentityFile(QString entry);
    QString sshConfigFile();
    QString sshConfigFile(QString entry);
    int sshPort();
    int sshPort(QString entry);
    QStringList sshOptions();
    QStringList sshOptions(QString entry);

protected:
    Config *m_config;
};

#endif // SYNCHRONIZECONFIG_H
