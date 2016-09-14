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

#include <QEventLoop>

#include "jobsrunner.h"

JobsRunner::JobsRunner(JobsLoader *loader, Logger *logger, QObject *parent) : QObject(parent)
{
    this->m_loader = loader;
    this->m_logger = logger;
}

void JobsRunner::run(QString name, QStringList entries)
{
    name = name.toLower();

    Job *job = this->m_loader->getLoadedJobs().value(name);

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

void JobsRunner::runAll(QString data)
{
    foreach(Job *job, this->m_loader->getLoadedJobs().values()) {
        this->m_logger->debug("Running job with arg: " + data);

        job->run(data);
    }
}
