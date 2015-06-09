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

#include <QDir>
#include <QCoreApplication>

#include <string.h>

#include "watcher/inotify/inotifywatcher.h"
#include "job/job.h"

INotifyWatcher::INotifyWatcher(Logger *logger, QObject *parent) :
    QObject(parent)
{
    this->m_logger = logger;
}

bool INotifyWatcher::init()
{
    this->m_dirs.removeDuplicates();

    foreach(QString dir, this->m_dirs) {
        if(!QDir(dir).exists()) {
            this->m_logger->log(tr("Dir not found: %1. Skipped.").arg(dir));
            this->m_dirs.removeOne(dir);
        }
    }

    if(this->m_dirs.empty()) {
        this->m_logger->log("Dirs not specified.");
        return false;
    }

    this->m_watcherThread = new INotifyThread(this->m_dirs);

    connect(this->m_watcherThread, SIGNAL(fileChanged(QString)), this, SIGNAL(fileChanged(QString)));
    connect(this->m_watcherThread, SIGNAL(watchAdded(QString)), this, SLOT(slot_watchAdded(QString)));
    connect(this->m_watcherThread, SIGNAL(watchAddFailed(QString,int)), this, SLOT(slot_watchAddFailed(QString,int)));
    connect(this->m_watcherThread, SIGNAL(watchesAddDone()), this, SLOT(slot_watchAddDone()));

    connect(qApp, SIGNAL(aboutToQuit()), this->m_watcherThread, SLOT(slot_stop()));

    this->m_logger->log("Adding watchers...");

    this->m_watcherThread->start();

    return true;
}

void INotifyWatcher::addDirs(QStringList dirs)
{
    this->m_dirs.append(dirs);
}

void INotifyWatcher::slot_watchAdded(QString dir)
{
    if(this->m_dirs.contains(dir)) {
        this->m_logger->log(tr("Watcher added for dir: %1 - adding watchers for subdirs...").arg(dir));
    }
}

void INotifyWatcher::slot_watchAddFailed(QString dir, int error)
{
    this->m_logger->log(tr("Failed to add watcher for dir: %1 - %2").arg(dir).arg(strerror(error)));
}

void INotifyWatcher::slot_watchAddDone()
{
    this->m_logger->log("Adding watchers done");
    emit(initialized());
}
