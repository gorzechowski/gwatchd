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

#include <QDebug>
#include <QSystemSemaphore>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "application.h"
#include "job/jobsloader.h"
#include "job/jobscollector.h"
#include "job/jobsrunner.h"
#include "logger/loggercomposite.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "logger/decorator/loggerleveldecorator.h"
#include "watcher/watcher.h"
#include "notification/notificationmanager.h"
#include "notification/notifier/socketnotifier.h"
#include "notification/jobsnotificationmanager.h"
#include "socket/socketserver.h"

QSystemSemaphore semaphore("gwatchd");

Application::Application(CommandLineParser *parser, int &argc, char **argv) : QCoreApplication(argc, argv)
{
    this->m_parser = parser;

    this->setApplicationName("GWatchD");
    this->setApplicationVersion(VERSION);

    this->parseArguments();
}

void Application::removePidFile()
{
    QFile pidFile(this->m_parser->pidFile());

    pidFile.remove();
}

void Application::init(ApplicationConfig *config)
{
    switch(this->m_mode) {
        case Application::Standard:
            this->initStandardMode(config);
            break;

        case Application::Single:
            this->initSingleMode(config);
            break;

        case Application::Daemon:
            this->initDaemonMode(config);
            break;

        default:
            break;
    }
}

QString Application::configDir()
{
    return this->m_parser->configDir();
}

Logger* Application::getLogger(ApplicationConfig *config)
{
    LoggerLevelDecorator *fileLogger = new LoggerLevelDecorator(
        new LoggerTimestampDecorator(
            new FileLogger(config->logsDirPath() + "/gwatchd.log", config)
        )
    );

    LoggerLevelDecorator *simpleLogger = new LoggerLevelDecorator(
        new LoggerTimestampDecorator(
            new SimpleLogger()
        )
    );

    LoggerComposite *logger = new LoggerComposite();

    logger->add(fileLogger);
    logger->add(simpleLogger);

    logger->setDebug(this->isDebug());

    return logger;
}

void Application::parseArguments()
{
    this->m_parser->process(this->arguments());

    if(this->m_parser->isSetHelp()) {
        if(this->m_parser->isSetRun()) {
            this->showRunHelp();
        } else {
            this->showHelp();
        }

        ::exit(0);
    }

    if(this->m_parser->isSetVersion()) {
        this->showVersion();
        ::exit(0);
    }

    if(this->m_parser->isSetRun()) {
        this->m_mode = Application::Single;
    } else if(this->m_parser->isSetNoDaemon()) {
        this->m_mode = Application::Standard;
    } else {
        this->m_mode = Application::Daemon;
    }
}

void Application::initStandardMode(ApplicationConfig *config)
{
    Logger *logger = this->getLogger(config);

    JobsCollector *collector = new JobsCollector(
        config->fileInfo().path() + "/job",
        this->applicationDirPath() + "/jobs",
        logger
    );
    JobsLoader *loader = new JobsLoader(config, logger);
    JobsRunner *runner = new JobsRunner(loader, logger);
    JobsNotificationManager *manager = new JobsNotificationManager(logger);

    foreach(JobDescriptor descriptor, collector->collectedJobs()) {
        loader->loadJob(descriptor);
    }

    Watcher *watcher = new Watcher(logger);

    foreach(Job *job, loader->getLoadedJobs().values()) {
        watcher->addEntries(job->getEntries());

        QObject *jobInstance = dynamic_cast<QObject*>(job);

        connect(jobInstance, SIGNAL(started()), manager, SLOT(slot_jobStarted()));
        connect(jobInstance, SIGNAL(running(Payload*)), manager, SLOT(slot_jobRunning(Payload*)));
        connect(jobInstance, SIGNAL(finished(int)), manager, SLOT(slot_jobFinished(int)));
        connect(jobInstance, SIGNAL(runRequested(QString, Entry)), runner, SLOT(run(QString, Entry)));
        connect(jobInstance, SIGNAL(runRequested(QString, Predefine)), runner, SLOT(run(QString, Predefine)));
    }

    connect(watcher, SIGNAL(fileChanged(QString)), runner, SLOT(runAll(QString)));

    QEventLoop loop;

    connect(watcher, SIGNAL(initialized()), &loop, SLOT(quit()));

    if(watcher->init()) {
        loop.exec();
    }

    NotificationManager *notificationManager = new NotificationManager();

    SocketServer *socketServer = new SocketServer(config, logger);

    socketServer->start();

    notificationManager->addNotifier(new SocketNotifier(socketServer));

    connect(manager, SIGNAL(notification(Notification*)), notificationManager, SLOT(slot_notification(Notification*)));

    #ifdef Q_OS_MAC
        QFile file(this->m_parser->pidFile());

        if(file.open(QIODevice::ReadOnly)) {
            if(file.readAll().toInt() == ::getpid()) {
                semaphore.release();

                ::close(STDIN_FILENO);
                ::close(STDOUT_FILENO);
                ::close(STDERR_FILENO);
            }

            file.close();
        }
    #endif
}

void Application::initSingleMode(ApplicationConfig *config)
{
    Logger *logger = this->getLogger(config);

    JobsCollector *collector = new JobsCollector(
        config->fileInfo().path() + "/job",
        this->applicationDirPath() + "/jobs",
        logger
    );
    JobsLoader *loader = new JobsLoader(config, logger);
    JobsRunner *runner = new JobsRunner(loader, logger);

    foreach(JobDescriptor descriptor, collector->collectedJobs()) {
        loader->loadJob(descriptor);
    }

    QString runJobName = this->m_parser->runJobName();
    QStringList runJobArgs = this->m_parser->runJobArgs();

    runner->run(runJobName, runJobArgs);

    ::exit(0);
}

void Application::initDaemonMode(ApplicationConfig *config)
{
    QFile pidFile;

    pid_t pid, sid;

    pid = ::fork();

    if(pid > 0) {
        // we are in parent process...

        pidFile.setFileName(this->m_parser->pidFile());

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

        semaphore.acquire();

        ::exit(0);
    }

    // we are in child process...

    #ifdef Q_OS_MAC
        char *debug = NULL;

        if(this->m_parser->isSetDebug()) {
            debug = QByteArray("-d").data();
        }

        char* const argv[] = {
            this->applicationFilePath().toUtf8().data(),
            QByteArray("--no-daemon").data(),
            debug,
            NULL
        };

        ::execv(this->applicationFilePath().toUtf8().data(), argv);

        ::exit(0);
    #endif

    umask(0);

    sid = ::setsid();

    if(sid < 0) {
        ::exit(1);
    }

    this->initStandardMode(config);

    semaphore.release();

    ::close(STDIN_FILENO);
    ::close(STDOUT_FILENO);
    ::close(STDERR_FILENO);
}

bool Application::isDaemon()
{
    return this->m_mode == Application::Daemon;
}

bool Application::isDebug()
{
    return this->m_parser->isSetDebug();
}

void Application::showVersion()
{
    printf(
        "%s %s Copyright (C) 2015 - 2016 Gracjan Orzechowski\n",
        qPrintable(this->applicationName()),
        qPrintable(this->applicationVersion())
    );
}

void Application::showHelp()
{
    printf(
        "Usage: gwatchd [options] [command]\n\n"

        "Options:\n"
        "  -p, --pid-file <file_path>    Set PID file path.\n"
        "  -c, --config-dir <dir_path>   Set config dir path.\n"
        "  --no-daemon                   Do not detach and logs to stdout/stderr.\n"
        "  -d, --debug                   Run with debug/verbose mode.\n\n"

        "  -h, --help                    Displays this help.\n"
        "  -v, --version                 Displays version information.\n\n"

        "Commands:\n"
        "  run <job_name> [<args>]       Run given job and quit.\n\n"

        "License:\n"
        "  GPLv2 or any later version.\n"
    );
}

void Application::showRunHelp()
{
    printf(
        "Usage: gwatchd [options] run <job_name> [<args>]\n\n"

        "Options:\n"
        "  --config-dir <dir_path>   Set config dir path\n\n"

        "License:\n"
        "  GPLv2 or any later version.\n"
    );
}
