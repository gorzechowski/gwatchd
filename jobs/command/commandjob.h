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

#ifndef COMMANDJOB_H
#define COMMANDJOB_H

#include <QString>
#include <QList>
#include <QProcess>
#include <QTimer>

#include "../../core/job/job.h"
#include "config/settings/hookdescriptor.h"
#include "job/entrylist.h"

class CommandJob : public QObject, public Job
{

    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "job/1.0" FILE "commandjob.json")
    Q_INTERFACES(Job)

public:
    CommandJob();

    void run(Entry entry);
    void run(Predefine predefine);

protected:
    QHash<QString, QProcess*> m_activeProcessList;

    QTimer *m_entryTimer;
    QTimer *m_predefineTimer;
    EntryList m_entries;
    QList<Predefine> m_predefines;

    QHash<QString, EntryList> m_files;

    void runHooks(QList<HookDescriptor> hooks, EntryList list);

    QString getCommand(QProcess *process);

    void execute(EntryList entries);
    void execute(QList<Predefine> predefines);

private slots:
    void slot_start();
    void slot_finished(int code);
    void slot_read();

    void execute();

signals:
    void started();
    void running(Payload*);
    void finished(int);
    void runRequested(QString, Entry);
    void runRequested(QString, Predefine);
};

#endif // COMMANDJOB_H
