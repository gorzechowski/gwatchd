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
#include "config/yamlconfig.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/loggercomposite.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "notification/statenotification.h"
#include "notification/factory/statenotificationfactory.h"

JobManager::JobManager(Config *config, QObject *parent) :
    QObject(parent)
{
    this->m_config = config;
}

void JobManager::loadAvailableJobs()
{
    foreach(JobManager::availableJob job, this->getAvailableJobs()) {
        this->loadJob(job);
    }
}

bool JobManager::loadJob(JobManager::availableJob job)
{
    QPluginLoader loader(job.value("pluginPath"));

    QObject *jobInstance = loader.instance();

    if(jobInstance) {
        Job *loadedJob = dynamic_cast<Job*>(jobInstance);

        if(loadedJob) {
            QString logDirPath = this->m_config->value("log.dirPath", "logs").toString();

            QJsonObject metaData = loader.metaData().value("MetaData").toObject();
            YamlConfig *config = new YamlConfig(job.value("configPath"));

            LoggerTimestampDecorator *fileLogger = new LoggerTimestampDecorator(
                new FileLogger(
                    QString("%1/job/%2.log").arg(logDirPath).arg(job.value("name")),
                    config
                )
            );

            LoggerTimestampDecorator *simpleLogger = new LoggerTimestampDecorator(new SimpleLogger());

            LoggerComposite *logger = new LoggerComposite();

            logger->add(fileLogger);
            logger->add(simpleLogger);

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

            return true;
        }
    }

    return false;
}

QList<JobManager::availableJob> JobManager::getAvailableJobs()
{
    QList<JobManager::availableJob> availableJobs;
    JobManager::availableJob job;

    QDir configsDir(this->m_config->fileInfo().path() + "/job");
    QDir jobsDir = qApp->applicationDirPath() + QString("/jobs");

    QStringList jobs = jobsDir.entryList(QDir::Files | QDir::Readable);

    foreach(QString file, configsDir.entryList(QDir::Files | QDir::Readable)) {
        if(!file.contains(QRegExp("^\\w+\\.yml$"))) {
            continue;
        }

        job.clear();

        job.insert("configPath", configsDir.absolutePath() + "/" + file);

        file.remove(".yml");

        file = jobs.at(jobs.indexOf(QRegExp(QString("^lib%1.*").arg(file))));

        QFile libFile(
            jobsDir.absolutePath() + QString("/%1").arg(file)
        );

        if(libFile.open(QIODevice::ReadOnly) && libFile.isReadable()) {
            job.insert("pluginPath", libFile.fileName());
            job.insert("name", file);

            availableJobs << job;
        }
    }

    return availableJobs;
}

QHash<QString, Job*> JobManager::getLoadedJobs()
{
    return this->m_loaded;
}

void JobManager::runJob(QString name, QStringList dirs)
{
    Job *job = this->getLoadedJobs().value(name.toLower());

    if(!job) {
        return;
    }

    QObject *jobObject = dynamic_cast<QObject*>(job);
    QEventLoop loop;

    connect(jobObject, SIGNAL(finished(int)), &loop, SLOT(quit()));

    if(dirs.isEmpty()) {
        dirs = job->getDirs();
    }

    foreach(QString dir, dirs) {
        if(!dir.endsWith("/")) {
            dir.append("/");
        }

        job->run(dir);
    }

    loop.exec();
}

void JobManager::slot_runJobs(QString data)
{
    foreach(Job *job, this->getLoadedJobs().values()) {
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
    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()))));
}

void JobManager::slot_jobRunning(Payload *payload)
{
    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), payload)));
}

void JobManager::slot_jobFinished(int code)
{
    bool success = code == 0 ? true : false;

    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), success)));
}
