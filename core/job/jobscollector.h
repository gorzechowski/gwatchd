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

#ifndef JOBSCOLLECTOR_H
#define JOBSCOLLECTOR_H

#include <QObject>

#include "job/jobdescriptor.h"
#include "logger/logger.h"
#include "config/applicationconfig.h"

class JobsCollector : public QObject
{
    Q_OBJECT
public:
    JobsCollector(QString configsDirPath, QString jobsDirPath, Logger *logger, QObject *parent = 0);

    QList<JobDescriptor> collectedJobs();

protected:
    ApplicationConfig *m_config;
    Logger *m_logger;
    QList<JobDescriptor> m_collected;

    void collectJobs(QString configsDirPath, QString jobsDirPath);
};

#endif // JOBSCOLLECTOR_H
