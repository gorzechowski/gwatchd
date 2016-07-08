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

class KQueueThread : public QThread
{
    Q_OBJECT
public:
    KQueueThread(QStringList dirs, QObject *parent = 0);

    void run();

protected:
    QStringList m_dirs;

    QMap<int, QString> m_watches;

    int m_kq;

    QStringList findNewEntries(QString);
    QStringList getEntriesForDir(QString, QMap<int, QString>);
    QStringList getEntriesForDir(QString);
    bool addWatcher(QString, bool emitSignal = true);

signals:
    void fileChanged(QString data);
    void watchesAddDone();
    void watchAdded(QString dir);
    void watchAddFailed(QString dir, int error);

public slots:
    void slot_stop();
};

#endif // KQUEUETHREAD_H
