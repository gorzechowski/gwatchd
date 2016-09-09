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

#include <QPluginLoader>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

#include "job/jobmanager.h"
#include "job/job.h"
#include "config/jsonconfig.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/loggercomposite.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "logger/decorator/loggerleveldecorator.h"
#include "notification/statenotification.h"
#include "notification/factory/statenotificationfactory.h"

JobManager::JobManager(bool isDebug, Logger *logger, ApplicationConfig *config, QObject *parent) :
    QObject(parent)
{
    this->m_isDebug = isDebug;
    this->m_logger = logger;
    this->m_config = config;
}

bool JobManager::loadJob(JobDescriptor jobDescriptor)
{
    QPluginLoader loader(jobDescriptor.pluginPath());

    this->m_logger->debug("Loading job: " + jobDescriptor.name());

    QObject *jobInstance = loader.instance();

    if(jobInstance) {
        Job *loadedJob = dynamic_cast<Job*>(jobInstance);

        if(loadedJob) {
            this->m_logger->debug("Initializing job");

            QString logDirPath = this->m_config->logsDirPath();

            QJsonObject metaData = loader.metaData().value("MetaData").toObject();
            JsonConfig *config = new JsonConfig(jobDescriptor.configPath());

            LoggerLevelDecorator *fileLogger = new LoggerLevelDecorator(
                new LoggerTimestampDecorator(
                    new FileLogger(
                        QString("%1/job/%2.log").arg(logDirPath).arg(jobDescriptor.name()),
                        this->m_config
                    )
                )
            );

            LoggerLevelDecorator *simpleLogger = new LoggerLevelDecorator(
                new LoggerTimestampDecorator(new SimpleLogger())
            );

            LoggerComposite *logger = new LoggerComposite();

            logger->add(fileLogger);
            logger->add(simpleLogger);

            logger->setDebug(this->m_isDebug);

            loadedJob->setConfig(config);
            loadedJob->setLogger(logger);

            jobInstance->setProperty("metaData", metaData);

            connect(jobInstance, SIGNAL(started()), this, SLOT(slot_jobStarted()));
            connect(jobInstance, SIGNAL(running(Payload*)), this, SLOT(slot_jobRunning(Payload*)));
            connect(jobInstance, SIGNAL(finished(int)), this, SLOT(slot_jobFinished(int)));

            this->m_loaded.insert(
                metaData.value("name").toString(),
                loadedJob
            );

            this->m_logger->debug("Job loaded");

            return true;
        }
    } else {
        this->m_logger->debug("Job not loaded: " + loader.errorString());
    }

    return false;
}

QHash<QString, Job*> JobManager::getLoadedJobs()
{
    return this->m_loaded;
}

void JobManager::runJob(QString name, QStringList entries)
{
    name = name.toLower();

    Job *job = this->getLoadedJobs().value(name);

    if(!job) {
        this->m_logger->debug(QString("Job %1 not found").arg(name));
        return;
    }

    QObject *jobObject = dynamic_cast<QObject*>(job);
    QEventLoop loop;

    connect(jobObject, SIGNAL(finished(int)), &loop, SLOT(quit()));

    if(entries.isEmpty()) {
        entries = job->getEntries();
    }

    foreach(QString entry, entries) {
        QFileInfo info(entry);

        if(info.isDir() && !entry.endsWith("/")) {
            entry.append("/");
        }

        this->m_logger->debug("Running job with arg: " + entry);

        job->run(entry);
    }

    loop.exec();
}

void JobManager::slot_runJobs(QString data)
{
    foreach(Job *job, this->getLoadedJobs().values()) {
        this->m_logger->debug("Running job with arg: " + data);

        job->run(data);
    }
}

QString JobManager::getJobName(QObject *job)
{
    QJsonObject data = job->property("metaData").toJsonObject();

    return data.value("name").toString();
}

void JobManager::slot_jobStarted()
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Started job " + name);

    emit(notification(StateNotificationFactory::create(name)));
}

void JobManager::slot_jobRunning(Payload *payload)
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Still running job " + name);

    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), payload)));
}

void JobManager::slot_jobFinished(int code)
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Finished job " + name + " with code " + QString::number(code));

    bool success = code == 0 ? true : false;

    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), success)));
}
