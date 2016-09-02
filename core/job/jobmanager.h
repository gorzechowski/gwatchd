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

#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <QObject>
#include <QHash>

#include "job/job.h"
#include "config/applicationconfig.h"
#include "logger/logger.h"
#include "notification/notification.h"
#include "notification/payload.h"

class JobManager : public QObject
{
    Q_OBJECT
public:
    typedef QHash<QString, QString> availableJob;

    JobManager(bool isDebug, Logger *logger, ApplicationConfig *config, QObject *parent = 0);

    QHash<QString, Job*> getLoadedJobs();
    void loadAvailableJobs();

    void runJob(QString name, QStringList dirs);

protected:
    bool loadJob(JobManager::availableJob job);
    QList<JobManager::availableJob> getAvailableJobs();
    QString getJobName(QObject*);

    QHash<QString, Job*> m_loaded;

    bool m_isDebug;
    Logger *m_logger;
    ApplicationConfig *m_config;

public slots:
    void slot_runJobs(QString data);

    void slot_jobStarted();
    void slot_jobRunning(Payload*);
    void slot_jobFinished(int);

signals:
    void notification(Notification*);
};

#endif // JOBMANAGER_H
