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

#ifndef RSYNCSETTINGS_H
#define RSYNCSETTINGS_H

#include <QString>
#include <QStringList>

class RsyncSettings
{
public:
    RsyncSettings(
        QString source,
        QStringList excludes,
        QStringList includes,
        QStringList targetHosts,
        QString targetPath,
        QString targetUser
    );

    QString source();
    QStringList excludes();
    QStringList includes();
    QStringList targetHosts();
    QString targetPath();
    QString targetUser();

protected:
    QString m_source;
    QStringList m_excludes;
    QStringList m_includes;
    QStringList m_targetHosts;
    QString m_targetPath;
    QString m_targetUser;
};

#endif // RSYNCSETTINGS_H
