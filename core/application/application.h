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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

#include "application/commandlineparser.h"
#include "config/applicationconfig.h"
#include "logger/logger.h"

class Application: public QCoreApplication
{
public:
    Application(CommandLineParser *parser, int &argc, char **argv);

    enum Mode {
        Daemon = 0,
        Standard,
        Single
    };

    void init(ApplicationConfig *config);

    void removePidFile();

    QString configDir();

    bool isDaemon();
    bool isDebug();

protected:
    CommandLineParser *m_parser;
    Logger *m_logger;

    int m_mode;

    Logger* getLogger(ApplicationConfig *config);

    void parseArguments();

    void unixSignalHandler();

    void initStandardMode(ApplicationConfig *config);
    void initSingleMode(ApplicationConfig *config);
    void initDaemonMode(ApplicationConfig *config);

    void showVersion();
    void showHelp();
    void showRunHelp();

};

#endif // APPLICATION_H
