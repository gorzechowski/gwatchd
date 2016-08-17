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
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>

#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>

#include "watcher/inotifythread.h"

#define MAX_EVENTS 1024
#define LEN_NAME   16
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN    (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))
#define WATCH_FLAGS (IN_MODIFY | IN_DELETE | IN_CREATE | IN_MOVE)

INotifyThread::INotifyThread(QStringList entries, QObject *parent) :
    QThread(parent)
{
    this->m_entries = entries;
}

void INotifyThread::run()
{
    if(this->m_entries.isEmpty()) {
        return;
    }

    this->m_fd = inotify_init();
    int watchDescriptor = 0;

    foreach(QString entry, this->m_entries) {
        if(!entry.startsWith("/")) {
            continue;
        }

        QFileInfo info(entry);

        if(info.isDir() && !entry.endsWith("/")) {
            entry.append("/");
        }

        watchDescriptor = inotify_add_watch(this->m_fd, entry.toUtf8(), WATCH_FLAGS);

        if(watchDescriptor >= 0) {
            this->m_watches.insert(watchDescriptor, entry);
            emit(watchAdded(entry));
        } else {
            emit(watchAddFailed(entry, errno));
        }

        if(info.isFile()) {
            continue;
        }

        QDirIterator it(entry, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while(it.hasNext()) {
            QString subDir = it.next();

            if(!subDir.endsWith("/")) subDir.append("/");

            watchDescriptor = inotify_add_watch(this->m_fd, subDir.toUtf8(), WATCH_FLAGS);

            if(watchDescriptor >= 0) {
                this->m_watches.insert(watchDescriptor, subDir);
                emit(watchAdded(subDir));
            } else {
                emit(watchAddFailed(subDir, errno));
            }

        }
    }

    emit(watchesAddDone());

    int length;
    int i;
    char buffer[BUF_LEN];

    QMap<uint, QString> fileMoving;
    QString path;

    while(true) {
        i = 0;
        length = read(m_fd, buffer, BUF_LEN);

        while(i < length) {
            struct inotify_event *event = (struct inotify_event*) &buffer[i];

            switch(event->mask) {
                case IN_MODIFY:
                case IN_DELETE:
                case IN_CREATE:
                    emit(fileChanged(this->m_watches.value(event->wd) + QString(event->name)));
                    break;

                case IN_MOVED_FROM:
                    fileMoving.insert(event->cookie, QString(this->m_watches.value(event->wd) + event->name));
                    break;

                case IN_MOVED_TO:
                    emit(fileChanged(
                        fileMoving.value(event->cookie) + ">" + this->m_watches.value(event->wd) + QString(event->name)
                    ));

                    fileMoving.remove(event->cookie);
                    break;

                case IN_CREATE | IN_ISDIR:
                    path = this->m_watches.value(event->wd) + event->name;

                    if(!path.endsWith("/")) {
                        path.append("/");
                    }

                    watchDescriptor = inotify_add_watch(this->m_fd, path.toUtf8(), WATCH_FLAGS);

                    if(watchDescriptor >= 0) {
                        this->m_watches.insert(watchDescriptor, path);
                    }

                    emit(fileChanged(path));
                    break;

                case IN_DELETE | IN_ISDIR:
                    path = this->m_watches.value(event->wd) + event->name;

                    watchDescriptor = this->m_watches.key(path);

                    inotify_rm_watch(this->m_fd, watchDescriptor);
                    this->m_watches.remove(watchDescriptor);

                    emit(fileChanged(path));
                    break;

                default:
                    break;
            }

            i += (event->len + EVENT_SIZE);
        }
    }
}

void INotifyThread::slot_stop()
{
    if(!this->isRunning()) return;

    if(this->m_fd) {
        foreach(int wd, this->m_watches.keys()) {
            inotify_rm_watch(this->m_fd, wd);
        }

        close(this->m_fd);
    }

    this->terminate();
    this->wait();
}
