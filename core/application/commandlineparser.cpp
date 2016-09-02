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

#include <QCoreApplication>

#include "commandlineparser.h"

CommandLineParser::CommandLineParser() : QCommandLineParser(),
    m_noDaemon(QCommandLineOption("no-daemon", "Do not detach and logs to stdout/stderr.")),
    m_pidFile(QCommandLineOption(QStringList() << "p" << "pid-file", "Set PID file path.", "file_path")),
    m_configDir(QCommandLineOption(QStringList() << "c" << "config-dir", "Set config dir path.", "dir_path")),
    m_help(QCommandLineOption(QStringList() << "h" << "help", "Displays this help.")),
    m_version(QCommandLineOption(QStringList() << "v" << "version", "Displays version information.")),
    m_debug(QCommandLineOption(QStringList() << "d" << "debug", "Run with debug/verbose mode."))
{
    this->addPositionalArgument("run", "Run given job and quit.", "[run <job_name>]");

    this->addOption(this->m_noDaemon);
    this->addOption(this->m_pidFile);
    this->addOption(this->m_configDir);

    this->addOption(this->m_debug);
    this->addOption(this->m_help);
    this->addOption(this->m_version);
}

QString CommandLineParser::pidFile()
{
    QString path = this->value(this->m_pidFile);

    if(!path.isEmpty()) {
        return path;
    }

    return qApp->applicationDirPath() + "/gwatchd.pid";
}

QString CommandLineParser::configDir()
{
    QString path = this->value(this->m_configDir);

    if(!path.isEmpty()) {
        return path;
    }

    return qApp->applicationDirPath() + "/config";
}

QString CommandLineParser::runJobName()
{
    QStringList args = this->positionalArguments();

    if(args.count() >= 2) {
        return args.at(1);
    }

    return QString();
}

QStringList CommandLineParser::runJobArgs()
{
    QStringList args = this->positionalArguments();

    if(args.count() >= 3) {
        return args.mid(2);
    }

    return QStringList();
}

bool CommandLineParser::isSetRun()
{
    QStringList args = this->positionalArguments();

    if(args.count() > 0) {
        return args.first() == "run";
    }

    return false;
}

bool CommandLineParser::isSetNoDaemon()
{
    return this->isSet(this->m_noDaemon);
}

bool CommandLineParser::isSetPidFile()
{
    return this->isSet(this->m_pidFile);
}

bool CommandLineParser::isSetConfigDir()
{
    return this->isSet(this->m_configDir);
}

bool CommandLineParser::isSetHelp()
{
    return this->isSet(this->m_help);
}

bool CommandLineParser::isSetVersion()
{
    return this->isSet(this->m_version);
}

bool CommandLineParser::isSetDebug()
{
    return this->isSet(this->m_debug);
}
