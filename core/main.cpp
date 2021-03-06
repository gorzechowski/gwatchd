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

#include <QTime>
#include <sys/signal.h>

#include "application/application.h"
#include "application/commandlineparser.h"
#include "config/jsonconfig.h"
#include "config/applicationconfig.h"

Application *app;

void unixSignalHandler(int)
{
    if(app->isDaemon()) {
        app->removePidFile();
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    app = new Application(new CommandLineParser(), argc, argv);

    signal(SIGINT, unixSignalHandler);
    signal(SIGTERM, unixSignalHandler);
    signal(SIGTSTP, unixSignalHandler);

    qsrand(QTime::currentTime().second());

    ApplicationConfig *config = new ApplicationConfig(app->applicationDirPath(), new JsonConfig(app->configDir() + "/config.json"));

    app->init(config);

    return app->exec();
}
