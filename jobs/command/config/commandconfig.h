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

#ifndef COMMANDCONFIG_H
#define COMMANDCONFIG_H

#include <QObject>

#include "../../../core/config/config.h"

class CommandConfig : public QObject
{
    Q_OBJECT
public:
    CommandConfig(Config *config, QObject *parent = 0);

    QJsonValue value(QString key);

    void setGroup(QString group);

    QStringList entries();
    bool remote(QString entry);
    QString exec(QString entry);
    QString fileMask(QString entry);
    QStringList sshHosts();
    QStringList sshHosts(QString entry);
    QString sshUser();
    QString sshUser(QString entry);
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

    QString m_group;
};

#endif // COMMANDCONFIG_H
