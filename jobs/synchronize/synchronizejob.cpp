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

#include <QCryptographicHash>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSystemSemaphore>

#include "synchronizejob.h"
#include "command/rsync/rsynccommandbuilder.h"
#include "notification/runningpayload.h"
#include "config/settings/factory/rsyncsettingsfactory.h"
#include "config/settings/factory/sshsettingsfactory.h"
#include "config/settings/factory/hookssettingsfactory.h"
#include "config/settings/factory/settingsfactory.h"

SynchronizeJob::SynchronizeJob()
{
    this->m_entryTimer = new QTimer();
    this->m_predefineTimer = new QTimer();

    connect(this->m_entryTimer, SIGNAL(timeout()), this, SLOT(synchronize()));
    connect(this->m_predefineTimer, SIGNAL(timeout()), this, SLOT(synchronize()));
}

void SynchronizeJob::run(Entry entry)
{
    if(this->m_entries.indexOf(entry) < 0) {
        this->m_entries << entry;
    }

    if(this->m_entryTimer->isActive()) {
        this->m_entryTimer->stop();
    }

    this->m_entryTimer->start(this->m_config->value("delay").toInt(100));
}

void SynchronizeJob::run(Predefine predefine)
{
    if(this->m_predefines.indexOf(predefine) < 0) {
        this->m_predefines << predefine;
    }

    if(this->m_predefineTimer->isActive()) {
        this->m_predefineTimer->stop();
    }

    this->m_predefineTimer->start(this->m_config->value("delay").toInt(100));
}

void SynchronizeJob::synchronize()
{
    QTimer *timer = dynamic_cast<QTimer*>(this->sender());

    if(timer == this->m_entryTimer) {
        this->synchronize(this->m_entries.filterEntries(this->getEntries(), this->m_config));
    } else if(timer == this->m_predefineTimer) {
        this->synchronize(this->m_predefines);
    }
}

void SynchronizeJob::synchronize(EntryList entries)
{
    if(this->m_entryTimer->isActive()) {
        this->m_entryTimer->stop();
    }

    EntryList changed = this->m_entries;

    this->m_entries.clear();

    if(entries.isEmpty()) {
        this->m_logger->debug("Synchronize job has nothing to do");

        emit(finished(0));

        return;
    }

    emit(started());

    foreach(Entry entry, entries) {
        RsyncSettings rsyncSettings = RsyncSettingsFactory::create(entry, this->m_config);
        SshSettings sshSettings = SshSettingsFactory::create(entry, this->m_config);

        RsyncCommandBuilder builder(&rsyncSettings, &sshSettings);
        QStringList commands = builder.build();

        foreach(QString command, commands) {
            QString hash = QCryptographicHash::hash(command.toUtf8(), QCryptographicHash::Md5).toHex();

            if(!this->m_files.contains(hash)) {
                this->m_files.insert(hash, changed);
            }

            QProcess *process = new QProcess();

            process->setProcessChannelMode(QProcess::MergedChannels);

            if(this->m_activeProcessList.keys().contains(hash)) {
                this->m_logger->debug("Synchronize process already exists " + hash);

                process = this->m_activeProcessList.value(hash);

                if(process->state() == QProcess::Running) {
                    this->m_logger->debug("Stopping synchronize process");

                    disconnect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                    process->close();
                    connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));

                    this->m_logger->debug("Synchronize process stopped");
                }
            } else {
                this->m_logger->debug("Creating new synchronize process");

                connect(process, SIGNAL(started()), this, SLOT(slot_start()));
                connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                connect(process, SIGNAL(readyRead()), this, SLOT(slot_read()));

                process->setProperty("entry", entry);
                process->setProperty("hash", hash);

                this->m_activeProcessList.insert(hash, process);

                this->m_logger->debug("New synchronize process created");
            }

            this->m_logger->debug("Starting synchronize process");

            process->start(command);
        }
    }
}

void SynchronizeJob::synchronize(QList<Predefine> predefines)
{
    if(this->m_predefineTimer->isActive()) {
        this->m_predefineTimer->stop();
    }

    this->m_predefines.clear();

    if(predefines.isEmpty()) {
        this->m_logger->debug("Synchronize job has nothing to do");

        emit(finished(0));

        return;
    }

    emit(started());

    foreach(Predefine predefine, predefines) {
        RsyncSettings rsyncSettings = RsyncSettingsFactory::create(predefine, this->m_config);
        SshSettings sshSettings = SshSettingsFactory::create(predefine, this->m_config);

        RsyncCommandBuilder builder(&rsyncSettings, &sshSettings);
        QStringList commands = builder.build();

        foreach(QString command, commands) {
            QString hash = QCryptographicHash::hash(command.toUtf8(), QCryptographicHash::Md5).toHex();

            QProcess *process = new QProcess();

            process->setProcessChannelMode(QProcess::MergedChannels);

            if(this->m_activeProcessList.keys().contains(hash)) {
                this->m_logger->debug("Synchronize process already exists " + hash);

                process = this->m_activeProcessList.value(hash);

                if(process->state() == QProcess::Running) {
                    this->m_logger->debug("Stopping synchronize process");

                    disconnect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                    process->close();
                    connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));

                    this->m_logger->debug("Synchronize process stopped");
                }
            } else {
                this->m_logger->debug("Creating new synchronize process");

                connect(process, SIGNAL(started()), this, SLOT(slot_start()));
                connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                connect(process, SIGNAL(readyRead()), this, SLOT(slot_read()));

                process->setProperty("predefine", predefine);
                process->setProperty("hash", hash);

                this->m_activeProcessList.insert(hash, process);

                this->m_logger->debug("New synchronize process created");
            }

            this->m_logger->debug("Starting synchronize process");

            process->start(command);
        }
    }
}

void SynchronizeJob::runHooks(QList<HookDescriptor> hooks, EntryList list)
{
    foreach(HookDescriptor hook, hooks) {
        if(!hook.fileMask().isEmpty() && list.filter(QRegularExpression(hook.fileMask())).count() < 1) {
            continue;
        }

        QString hookKey = QString("%1:%2").arg(hook.jobName(), hook.predefine());

        this->m_logger->debug(QString("Requesting hook %1").arg(hookKey));

        QSystemSemaphore semaphore(hookKey);

        emit(runRequested(hook.jobName(), hook.predefine()));

        semaphore.acquire();

        this->m_logger->debug("Hook finished work");
    }
}

void SynchronizeJob::slot_start()
{
    QProcess *process = static_cast<QProcess*>(this->sender());
    QString data = process->property("entry").toString();

    if(data.isEmpty()) {
        data = process->property("predefine").toString();
    }

    this->m_logger->log(QString("Synchronizing %1...").arg(data));

    RunningPayload *payload = new RunningPayload();

    payload->addEntryInfo(data, RunningPayload::Started);

    emit(running(payload));
}

void SynchronizeJob::slot_finished(int code)
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    RunningPayload *payload = new RunningPayload();

    QString hash = process->property("hash").toString();
    Entry entry = process->property("entry").toString();
    Predefine predefine = process->property("predefine").toString();
    QString data;

    if(!entry.isEmpty()) {
        payload->addEntryInfo(entry, code > 0 ? RunningPayload::Failed : RunningPayload::Finished);
        data = entry;
    } else {
        payload->addEntryInfo(predefine, code > 0 ? RunningPayload::Failed : RunningPayload::Finished);
        data = predefine;
    }

    emit(running(payload));

    this->m_activeProcessList.remove(hash);

    if(code > 0) {
        this->m_logger->error(QString("Synchronizing %1 failed").arg(data));
    } else {
        this->m_logger->log(QString("Synchronizing %1 done").arg(data));
    }

    if(this->m_activeProcessList.isEmpty()) {
        emit(finished(code));

        this->m_logger->debug("Looking for hooks");

        HooksSettings hooksSettings;

        if(!entry.isEmpty()) {
            hooksSettings = HooksSettingsFactory::create(entry, this->m_config);
        } else {
            hooksSettings = HooksSettingsFactory::create(predefine, this->m_config);
        }

        QList<HookDescriptor> hooks = code > 0 ? hooksSettings.failedHooks() : hooksSettings.finishedHooks();

        if(hooks.count() >= 1) {
            this->runHooks(hooks, this->m_files.value(hash));

            this->m_files.remove(hash);
        } else {
            this->m_logger->debug("No hooks found");
        }
    }
}

void SynchronizeJob::slot_read()
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString data = QString(process->readAll()).trimmed().remove("\n");

    if(!data.isEmpty()) {
        this->m_logger->log(data);
    }
}
