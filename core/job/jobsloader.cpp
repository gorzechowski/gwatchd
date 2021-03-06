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

#include "jobsloader.h"

#include "config/jsonconfig.h"
#include "logger/factory/defaultloggerfactory.h"

JobsLoader::JobsLoader(ApplicationConfig *config, Logger *logger, QObject *parent) : QObject(parent)
{
    this->m_config = config;
    this->m_logger = logger;
}

QHash<QString, Job*> JobsLoader::getLoadedJobs()
{
    return this->m_loaded;
}

bool JobsLoader::loadJob(JobDescriptor jobDescriptor, bool isDebug)
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

            Logger *logger = DefaultLoggerFactory::create(
                QString("%1/job/%2.log").arg(logDirPath).arg(jobDescriptor.name()),
                this->m_config,
                isDebug
            );

            JsonConfig *config = new JsonConfig(jobDescriptor.configPath());
            config->addDeprecated(metaData.value("config").toObject().value("deprecated").toArray());
            config->setLogger(logger);

            loadedJob->setConfig(config);
            loadedJob->setLogger(logger);

            jobInstance->setProperty("metaData", metaData);

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
