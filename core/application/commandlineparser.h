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

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <QCommandLineOption>

class CommandLineParser : public QCommandLineParser
{
public:
    CommandLineParser();

    QString pidFile();
    QString configDir();
    QString runJobName();
    QStringList runJobArgs();

    bool isSetRun();
    bool isSetNoDaemon();
    bool isSetPidFile();
    bool isSetConfigDir();
    bool isSetHelp();
    bool isSetVersion();
    bool isSetDebug();

protected:
    QCommandLineOption m_noDaemon;
    QCommandLineOption m_pidFile;
    QCommandLineOption m_configDir;
    QCommandLineOption m_help;
    QCommandLineOption m_version;
    QCommandLineOption m_debug;
};

#endif // COMMANDLINEPARSER_H
