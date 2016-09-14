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

#ifndef SYNCHRONIZEJOB_H
#define SYNCHRONIZEJOB_H

#include <QString>
#include <QList>
#include <QProcess>
#include <QTimer>

#include "../../core/job/job.h"
#include "../../core/notification/payload.h"
#include "config/synchronizeconfig.h"

class SynchronizeJob : public QObject, public Job
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "job/1.0" FILE "synchronizejob.json")
    Q_INTERFACES(Job)

public:
    SynchronizeJob();

    QStringList getEntries();
    void run(QString data);
    void setConfig(Config *config);
    void setLogger(Logger *logger);

protected:
    SynchronizeConfig *m_config;
    QHash<QString, QProcess*> m_activeProcessList;

    QTimer *m_timer;
    QStringList m_files;

    QStringList retrieveEntries(QStringList files);

private slots:
    void slot_start();
    void slot_finished(int code);
    void slot_read();

    void slot_synchronize();

signals:
    void started();
    void running(Payload*);
    void finished(int);
};

#endif // SYNCHRONIZEJOB_H
