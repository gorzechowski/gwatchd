#include <QDebug>
#include <QSystemSemaphore>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "application.h"
#include "job/jobmanager.h"
#include "logger/loggercomposite.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "watcher/watcher.h"
#include "notification/notificationmanager.h"
#include "notification/notifier/socketnotifier.h"
#include "socket/socketserver.h"

Application::Application(CommandLineParser *parser, int &argc, char **argv) : QCoreApplication(argc, argv)
{
    this->m_parser = parser;

    this->parseArguments();
}

void Application::removePidFile()
{
    QFile pidFile(this->m_parser->pidFile());

    pidFile.remove();
}

void Application::init(Config *config)
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

void Application::parseArguments()
{
    this->m_parser->process(this->arguments());

    if(this->m_parser->isSetHelp()) {
        this->showHelp();
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

void Application::initStandardMode(Config *config)
{
    LoggerTimestampDecorator *fileLogger = new LoggerTimestampDecorator(
        new FileLogger(config->value("log.dirPath", "/var/log/gwatchd").toString() + "/gwatchd.log", config)
    );

    LoggerTimestampDecorator *simpleLogger = new LoggerTimestampDecorator(new SimpleLogger());

    LoggerComposite *logger = new LoggerComposite();

    logger->add(fileLogger);
    logger->add(simpleLogger);

    JobManager *manager = new JobManager(config);

    manager->loadAvailableJobs();

    Watcher *watcher = new Watcher(logger);

    foreach(Job *job, manager->getLoadedJobs().values()) {
        watcher->addDirs(job->getDirs());
    }

    connect(watcher, SIGNAL(fileChanged(QString)), manager, SLOT(slot_runJobs(QString)));

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
}

void Application::initSingleMode(Config *config)
{
    JobManager *manager = new JobManager(config);

    manager->loadAvailableJobs();

    QString runJobName = this->m_parser->runJobName();
    QStringList runJobArgs = this->m_parser->runJobArgs();

    manager->runJob(runJobName, runJobArgs);

    ::exit(0);
}

void Application::initDaemonMode(Config *config)
{
    QSystemSemaphore semaphore("gwatchd");
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
