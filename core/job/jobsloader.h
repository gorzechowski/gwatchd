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

#ifndef JOBSLOADER_H
#define JOBSLOADER_H

#include <QObject>

#include "config/applicationconfig.h"
#include "job/job.h"
#include "job/jobdescriptor.h"
#include "logger/logger.h"

class JobsLoader : public QObject
{
    Q_OBJECT
public:
    JobsLoader(ApplicationConfig *config, Logger *logger, QObject *parent = 0);

    QHash<QString, Job*> getLoadedJobs();

    bool loadJob(JobDescriptor jobDescriptor);

protected:
    ApplicationConfig *m_config;
    Logger *m_logger;

    QHash<QString, Job*> m_loaded;
};

#endif // JOBSLOADER_H
