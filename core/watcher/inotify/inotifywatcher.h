/*
 * Copyright (C) 2015 Gracjan Orzechowski
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

#ifndef INOTIFYWATCHER_H
#define INOTIFYWATCHER_H

#include <QObject>

#include "job/jobmanager.h"
#include "watcher/inotify/inotifythread.h"
#include "watcher/watcher.h"
#include "logger/logger.h"

class INotifyWatcher : public QObject, public Watcher
{
    Q_OBJECT
public:
    INotifyWatcher(Logger *logger, QObject *parent = 0);

    bool init();
    void addDirs(QStringList dirs);

protected:
    INotifyThread *m_watcherThread;

    QStringList m_dirs;
    Logger *m_logger;

signals:
    void fileChanged(QString data);
    void initialized();

private slots:
    void slot_watchAdded(QString dir);
    void slot_watchAddFailed(QString dir, int error);
    void slot_watchAddDone();
};

#endif // INOTIFYENGINE_H
