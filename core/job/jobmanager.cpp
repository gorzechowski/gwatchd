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

#include "job/jobmanager.h"
#include "job/job.h"
#include "config/yamlconfig.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/loggercomposite.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "notification/statusnotification.h"

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
            QString logDirPath = this->m_config->value("log.dirPath", "/var/log/gwatchd").toString();

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

void JobManager::startJob(QString name, QStringList dirs)
{
    Job *j = 0;

    foreach(Job *job, this->getLoadedJobs().values()) {
        if(this->m_loaded.key(job).toLower() == name.toLower()) {
            j = job;
            break;
        }
    }

    if(dirs.isEmpty()) {
        dirs = j->getDirs();
    }

    if(j) {
        foreach(QString dir, dirs) {
            if(!dir.endsWith("/")) {
                dir.append("/");
            }

            j->run(dir);
        }
    }
}

QList<JobManager::availableJob> JobManager::getAvailableJobs()
{
    QList<JobManager::availableJob> jobs;
    JobManager::availableJob job;

    QDir configsDir(this->m_config->fileInfo().path() + "/job");

    foreach(QString file, configsDir.entryList(QDir::Files | QDir::Readable)) {
        if(!file.contains(QRegExp("^\\w+\\.yml$"))) {
            continue;
        }

        job.clear();

        job.insert("configPath", configsDir.absolutePath() + "/" + file);

        file.remove(".yml");

        QString libDirPath = this->m_config->value("lib.dirPath", "/usr/lib/gwatchd").toString();

        QFile libFile(
            libDirPath + QString("/job/lib%1job.so").arg(file)
        );

        if(libFile.open(QIODevice::ReadOnly) && libFile.isReadable()) {
            job.insert("pluginPath", libFile.fileName());
            job.insert("name", file);
            jobs << job;
        }
    }

    return jobs;
}

QHash<QString, Job*> JobManager::getLoadedJobs()
{
    return this->m_loaded;
}

void JobManager::slot_runJobs(QString data)
{
    foreach(Job *job, this->getLoadedJobs().values()) {
        job->run(data);
    }
}

void JobManager::slot_jobStarted()
{
    QObject *job = static_cast<QObject*>(this->sender());
    QJsonObject data = job->property("metaData").toJsonObject();

    Notification *n = new StatusNotification(
        data.value("name").toString(),
        StatusNotification::Started
    );

    emit(notification(n));
}

void JobManager::slot_jobFinished(int code)
{
    QObject *job = static_cast<QObject*>(this->sender());
    QJsonObject data = job->property("metaData").toJsonObject();

    Notification *n = new StatusNotification(
        data.value("name").toString(),
        code == 0 ? StatusNotification::Finished : StatusNotification::Failed
    );

    emit(notification(n));
}
