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

#ifndef KQUEUETHREAD_H
#define KQUEUETHREAD_H

#include <QThread>
#include <QStringList>
#include <QMap>
#include <QHash>
#include <QTime>

#include "logger/logger.h"

class KQueueThread : public QThread
{
    Q_OBJECT
public:
    KQueueThread(QStringList entries, Logger *logger, QObject *parent = 0);

    void run();

protected:
    QStringList m_entries;
    Logger *m_logger;

    QMap<int, QString> m_watches;

    QHash<QString, QTime> m_debounce;

    int m_kq;

    QStringList findNewEntries(QString);
    QStringList getEntriesForDir(QString, QMap<int, QString>);
    QStringList getEntriesForDir(QString);
    bool addWatcher(QString);

    void watchAdded(QString entry);
    void watchAddFailed(QString entry, int error);

    void debounce(QString data);

signals:
    void fileChanged(QString data);
    void watchesAddDone();

public slots:
    void slot_stop();
};

#endif // KQUEUETHREAD_H
