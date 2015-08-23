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
#include <QDirIterator>
#include <QCoreApplication>
#include <QSet>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/event.h>
#include <string.h>

#include "watcher/kqueuethread.h"

#define FILTER_FLAGS (NOTE_WRITE | NOTE_DELETE | NOTE_RENAME)
#define FLAGS        (EV_ADD | EV_CLEAR | EV_ENABLE)

KQueueThread::KQueueThread(QStringList dirs, QObject *parent) :
    QThread(parent)
{
    this->m_dirs = dirs;
}

void KQueueThread::run()
{
    if(this->m_dirs.isEmpty()) {
        return;
    }

    this->m_kq = kqueue();

    foreach(QString dir, this->m_dirs) {
        if(!dir.startsWith("/")) {
            continue;
        }

        this->addWatcher(dir);

        QDirIterator it(dir, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while(it.hasNext()) {
            QString path = it.next();

            bool isFile = QFileInfo(path).isFile();

            if(!path.endsWith("/") && !isFile) path.append("/");

            this->addWatcher(path, !isFile);
        }
    }

    emit(watchesAddDone());

    while(true) {
        struct kevent event;

        if (kevent(this->m_kq, NULL, 0, &event, 1, NULL) == -1) {
            break;
        }

        if(event.udata == NULL) {
            continue;
        }

        int ident = (int) event.ident;
        QString path = this->m_watches.value(ident);

        if (event.fflags & NOTE_WRITE) {
            if(QFileInfo(path).isDir()) {
                QStringList entries = this->findNewEntries(path);

                foreach(QString entry, entries) {
                    this->addWatcher(entry, false);

                    emit(fileChanged(entry));
                }
            } else {
                emit(fileChanged(path));
            }
        }

        if(event.fflags & NOTE_DELETE || event.fflags & NOTE_RENAME) {
            emit(fileChanged(path));
            this->m_watches.remove(ident);
            close(ident);
        }
    }
}

bool KQueueThread::addWatcher(QString path, bool emitSignal)
{
    struct kevent direvent;
    const char *dirname = path.toUtf8();
    int dirDescriptor = open(path.toUtf8(), O_RDONLY);

    if(dirDescriptor <= -1) {
        emit(watchAddFailed(path, errno));
        return false;
    }

    EV_SET(
        &direvent,
        dirDescriptor,
        EVFILT_VNODE,
        FLAGS,
        FILTER_FLAGS,
        0,
        (void *) dirname
    );

    if(kevent(this->m_kq, &direvent, 1, NULL, 0, NULL) >= 0) {
        this->m_watches.insert(dirDescriptor, path);

        if(emitSignal) {
            emit(watchAdded(path));
        }
    } else {
        if(emitSignal) {
           emit(watchAddFailed(path, errno));
        }

        return false;
    }

    return true;
}

QStringList KQueueThread::findNewEntries(QString dir)
{
    QStringList watchedEntries = this->getEntriesForDir(dir, this->m_watches);
    QStringList entries = this->getEntriesForDir(dir);

    QSet<QString> diff = entries.toSet().subtract(watchedEntries.toSet());

    return QStringList::fromSet(diff);
}

QStringList KQueueThread::getEntriesForDir(QString dir, QMap<int, QString> watchers)
{
    QStringList result;

    int dirParts = dir.split("/", QString::SkipEmptyParts).count();

    foreach(QString path, watchers.values()) {
        int pathParts = path.split("/", QString::SkipEmptyParts).count();
     
        if(path.startsWith(dir) && (pathParts - dirParts) == 1) result << path;
    }

    return result;
}

QStringList KQueueThread::getEntriesForDir(QString dir)
{
    QStringList result;

    QDirIterator it(dir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    while(it.hasNext()) {
        QString path = it.next();

        if(QFileInfo(path).isDir()) path.append("/");

        result << path;
    }

    return result;
}

void KQueueThread::slot_stop()
{
    if(!this->isRunning()) return;

    if(this->m_kq) {
        foreach(int wd, this->m_watches.keys()) {
            close(wd);
        }

        close(this->m_kq);
    }

    this->terminate();
    this->wait();
}
