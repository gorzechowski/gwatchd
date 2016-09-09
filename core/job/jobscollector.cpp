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

#include <QDir>
#include <QFile>
#include <QCoreApplication>

#include "jobscollector.h"

JobsCollector::JobsCollector(ApplicationConfig *config, Logger *logger, QObject *parent) : QObject(parent)
{
    this->m_config = config;
    this->m_logger = logger;

    this->collectJobs();
}

void JobsCollector::collectJobs()
{
    QDir configsDir(this->m_config->fileInfo().path() + "/job");
    QDir jobsDir = qApp->applicationDirPath() + QString("/jobs");

    this->m_logger->debug("Looking for jobs in: " + jobsDir.absolutePath());
    this->m_logger->debug("Looking for job configs in: " + configsDir.absolutePath());

    QStringList jobs = jobsDir.entryList(QDir::Files | QDir::Readable);

    QString name, configPath, pluginPath;

    foreach(QString file, configsDir.entryList(QStringList() << "*.json", QDir::Files | QDir::Readable)) {
        configPath = configsDir.absolutePath() + "/" + file;

        file.remove(".json");

        name = file;

        file = jobs.at(jobs.indexOf(QRegExp(QString("^lib%1.*").arg(file))));

        QFile libFile(
            jobsDir.absolutePath() + QString("/%1").arg(file)
        );

        if(libFile.open(QIODevice::ReadOnly) && libFile.isReadable()) {
            pluginPath = libFile.fileName();

            this->m_logger->debug("Found job " + name);

            this->m_collected << JobDescriptor(name, configPath, pluginPath);

            libFile.close();
        }
    }
}

QList<JobDescriptor> JobsCollector::collectedJobs()
{
    return this->m_collected;
}
