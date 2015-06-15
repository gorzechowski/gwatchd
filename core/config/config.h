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

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QFileInfo>

class Config : public QObject
{
public:
    Config(QString filePath, QObject *parent = 0) : QObject(parent), m_fileInfo(QFileInfo(filePath)) {}
    virtual ~Config() {}

    virtual QVariant value(QString key, QVariant defaultValue = QVariant()) = 0;
    virtual QStringList listValue(QString key, QStringList defaultValue = QStringList()) = 0;
    QFileInfo fileInfo() { return m_fileInfo; }

protected:
    QFileInfo m_fileInfo;
};

#endif // CONFIG_H
