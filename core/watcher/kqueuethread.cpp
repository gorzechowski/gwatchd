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
#include <QDebug>

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
    int dirDescriptor;
    struct kevent direvent;
    const char *dirname;
qDebug() << "dirs" << this->m_dirs;
    foreach(QString dir, this->m_dirs) {
        if(!dir.startsWith("/")) {
            continue;
        }

        dirname = dir.toUtf8();
        dirDescriptor = open(dir.toUtf8(), O_RDONLY);
qDebug() << dir << dirDescriptor << strerror(errno);
        if(dirDescriptor <= -1) {
            continue;
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
            this->m_watches.insert(dirDescriptor, dir);
            emit(watchAdded(dir));
        } else {
            emit(watchAddFailed(dir, errno));
        }

        QDirIterator it(dir, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while(it.hasNext()) {
            QString subDir = it.next();

            bool isFile = QFileInfo(subDir).isFile();

            if(!subDir.endsWith("/") && !isFile) subDir.append("/");

            dirname = subDir.toUtf8();
            dirDescriptor = open(subDir.toUtf8(), O_RDONLY);

            if(dirDescriptor <= -1) {
                continue;
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
                this->m_watches.insert(dirDescriptor, subDir);

                if(!isFile) {
                    emit(watchAdded(subDir));
                }
            } else {
                if(!isFile) {
                    emit(watchAddFailed(subDir, errno));
                }
            }
        }
    }

    emit(watchesAddDone());

    while(true) {
        struct kevent event;

        if (kevent(this->m_kq, NULL, 0, &event, 1, NULL) == -1) {
            qApp->quit();
            break;
        }

        if (event.udata != NULL) {
            emit(fileChanged(this->m_watches.value((int) event.ident)));
        }
    }
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
