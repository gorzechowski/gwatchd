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

SynchronizeJob::SynchronizeJob()
{
    this->m_timer = new QTimer();

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(synchronize()));
}

void SynchronizeJob::run(Entry entry)
{
    if(this->m_files.indexOf(entry) < 0) {
        this->m_files << entry;
    }

    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    this->m_timer->start(this->m_config->value("delay").toInt(100));
}

void SynchronizeJob::run(Predefine)
{
    this->m_logger->log("Synchronize job does not support predefines in this moment");
    return;
}

void SynchronizeJob::synchronize()
{
    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    QStringList entries = this->retrieveEntries(this->m_files);

    this->m_files.clear();

    if(entries.isEmpty()) {
        this->m_logger->debug("Synchronize job has nothing to do");

        emit(finished(0));

        return;
    }

    emit(started());

    foreach(QString entry, entries) {
        RsyncSettings rsyncSettings = RsyncSettingsFactory::create(Entry(entry), this->m_config);
        SshSettings sshSettings = SshSettingsFactory::create(Entry(entry), this->m_config);

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

QStringList SynchronizeJob::retrieveEntries(QStringList files)
{
    QStringList entries;

    foreach(QString entry, this->getEntries()) {
        foreach(QString file, files) {
            if(file.startsWith(entry)) {
                QString fileMask = this->m_config->value("dirs").toObject().value(entry).toObject().value("fileMask").toString();

                if(!fileMask.isEmpty()) {
                    QString fileName = file.split("/").last();
                    QRegularExpression regex(fileMask);
                    QRegularExpressionMatch match = regex.match(fileName);

                    if(!match.hasMatch()) {
                        continue;
                    }
                }

                entries << entry;
                break;
            }
        }
    }

    return entries;
}

void SynchronizeJob::runHooks(QList<HookDescriptor> hooks)
{
    foreach(HookDescriptor hook, hooks) {
        QSystemSemaphore semaphore(QString("%1:%2").arg(hook.jobName(), hook.predefine()));

        emit(runRequested(hook.jobName(), hook.predefine()));

        semaphore.acquire();
    }
}

void SynchronizeJob::slot_start()
{
    QProcess *process = static_cast<QProcess*>(this->sender());
    QString entry = process->property("entry").toString();

    this->m_logger->log(QString("Synchronizing %1...").arg(entry));

    RunningPayload *payload = new RunningPayload();

    payload->addEntryInfo(entry, RunningPayload::Started);

    emit(running(payload));
}

void SynchronizeJob::slot_finished(int code)
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString entry = process->property("entry").toString();
    RunningPayload *payload = new RunningPayload();

    payload->addEntryInfo(entry, code > 0 ? RunningPayload::Failed : RunningPayload::Finished);

    emit(running(payload));

    this->m_activeProcessList.remove(process->property("hash").toString());

    if(code > 0) {
        this->m_logger->error(QString("Synchronizing %1 failed").arg(entry));
    } else {
        this->m_logger->log(QString("Synchronizing %1 done").arg(entry));
    }

    if(this->m_activeProcessList.isEmpty()) {
        emit(finished(code));

        HooksSettings hooksSettings = HooksSettingsFactory::create(Entry(entry), this->m_config);
        QList<HookDescriptor> hooks = code > 0 ? hooksSettings.failedHooks() : hooksSettings.finishedHooks();

        this->runHooks(hooks);
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
