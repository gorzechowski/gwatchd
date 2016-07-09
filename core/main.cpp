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
#include <QObject>
#include <QTime>
#include <QEventLoop>
#include <QFileInfo>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "job/jobmanager.h"
#include "config/yamlconfig.h"
#include "logger/filelogger.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "watcher/watcher.h"
#include "notification/notificationmanager.h"
#include "notification/notifier/socketnotifier.h"
#include "socket/socketserver.h"

QFile pidFile;

void showVersion()
{
    printf(
        "GWatchD %s Copyright (C) 2015 - 2016 Gracjan Orzechowski\n",
        qPrintable(qApp->applicationVersion())
    );
}

void showHelp()
{
    printf(
        "Usage: gwatchd [--pid-file <file_path>] [--config-dir <dir_path>] [--no-daemon]\n\n"

        "Options:\n"
        "  --pid-file <file_path>    Set PID file path\n"
        "  --config-dir <dir_path>   Set config dir path\n"
        "  --no-daemon               Do not detach and logs to stdout/stderr\n\n"

        "  --help                    Print options\n"
        "  --version                 Print version\n\n"

        "License:\n"
        "  GPLv2 or any later version.\n"
    );
}

void unixSignalHandler(int)
{
    if(qApp->property("isDaemon").toBool()) {
        pidFile.remove();
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setApplicationName("GWatchD");
    app.setApplicationVersion("1.0.3");

    if(app.arguments().contains("--help")) {
        showHelp();
        exit(0);
    }

    if(app.arguments().contains("--version")) {
        showVersion();
        exit(0);
    }

    if(app.arguments().contains("--pid-file")) {
        int index = app.arguments().indexOf("--pid-file") + 1;

        if(app.arguments().count() - 1 < index) {
            printf("--pid-file requires 1 argument\n");
            exit(1);
        }

        pidFile.setFileName(app.arguments().at(index));
    } else {
        pidFile.setFileName("/var/run/gwatchd.pid");
    }

    QString configDirPath;

    if(app.arguments().contains("--config-dir")) {
        int index = app.arguments().indexOf("--config-dir") + 1;

        if(app.arguments().count() - 1 < index) {
            printf("--config-file requires 1 argument\n");
            exit(1);
        }

        configDirPath = app.arguments().at(index);
    } else {
        configDirPath = "/etc/gwatchd";
    }

    app.setProperty("isDaemon", !app.arguments().contains("--no-daemon"));
    app.setProperty("stdoutAvailable", true);

    int pipefd[2];
    pid_t pid, sid;
    char buf[6];
    char pipeMsg[6] = "ready";

    if(app.property("isDaemon").toBool()) {
        pipe(pipefd);

        pid = fork();

        if(pid > 0) {
            if(!pidFile.open(QIODevice::WriteOnly)) {
                printf(
                    "Could not create PID file %s: %s\n",
                    qPrintable(pidFile.fileName()),
                    qPrintable(pidFile.errorString())
                );
            } else {
                pidFile.write(QString::number(pid).toUtf8());
                pidFile.flush();
                pidFile.close();
            }


            close(pipefd[1]);

            while(read(pipefd[0], &buf, sizeof(buf)) <= 0);

            close(pipefd[1]);

            if(strcmp(buf, pipeMsg) == 0) {
                exit(0);
            } else {
                exit(1);
            }
        } else if(pid < 0) {
            exit(1);
        }

        close(pipefd[0]);

        umask(0);

        sid = setsid();

        if(sid < 0) {
            exit(1);
        }
    }

    qsrand(QTime::currentTime().second());

    signal(SIGINT, unixSignalHandler);
    signal(SIGTERM, unixSignalHandler);
    signal(SIGTSTP, unixSignalHandler);

    YamlConfig *config = new YamlConfig(configDirPath + "/config.yml");

    LoggerTimestampDecorator *logger = new LoggerTimestampDecorator(
        new FileLogger(config->value("log.dirPath", "/var/log/gwatchd").toString() + "/gwatchd.log", config)
    );

    JobManager *manager = new JobManager(config);

    Watcher *watcher = new Watcher(logger);

    manager->loadAvailableJobs();

    foreach(Job *job, manager->getLoadedJobs().values()) {
        watcher->addDirs(job->getDirs());
    }

    QObject::connect(watcher, SIGNAL(fileChanged(QString)), manager, SLOT(slot_runJobs(QString)));

    QEventLoop loop;

    QObject::connect(watcher, SIGNAL(initialized()), &loop, SLOT(quit()));

    if(watcher->init()) {
        loop.exec();
    }

    NotificationManager *notificationManager = new NotificationManager();

    SocketServer *socketServer = new SocketServer(config, logger);

    socketServer->start();

    notificationManager->addNotifier(new SocketNotifier(socketServer));

    QObject::connect(manager, SIGNAL(notification(Notification*)), notificationManager, SLOT(slot_notification(Notification*)));

    if(app.property("isDaemon").toBool()) {
        write(pipefd[1], pipeMsg, sizeof(pipeMsg));

        close(pipefd[1]);

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        app.setProperty("stdoutAvailable", false);
    }

    return app.exec();
}
