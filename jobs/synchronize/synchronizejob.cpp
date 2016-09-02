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

#include <QDebug>
#include <QCryptographicHash>
#include <QFileInfo>

#include "synchronizejob.h"
#include "command/rsync/rsynccommandbuilder.h"
#include "notification/runningpayload.h"
#include "config/synchronizeconfig.h"

SynchronizeJob::SynchronizeJob()
{
    this->m_timer = new QTimer();

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(slot_synchronize()));
}

void SynchronizeJob::setConfig(Config *config)
{
    this->m_config = new SynchronizeConfig(config);
}

void SynchronizeJob::setLogger(Logger *logger)
{
    this->m_logger = logger;
}

QStringList SynchronizeJob::getEntries()
{
    return this->m_config->entries();
}

void SynchronizeJob::run(QString data)
{
    this->m_files << data;

    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    this->m_timer->start(this->m_config->value("delay").toInt(100));
}

void SynchronizeJob::slot_synchronize()
{
    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    emit(started());

    QStringList entries;

    foreach(QString entry, this->getEntries()) {
        foreach(QString file, this->m_files) {
            if(file.startsWith(entry)) {
                entries << entry;
                break;
            }
        }
    }

    this->m_files.clear();

    foreach(QString entry, entries) {
        QFileInfo info(entry);

        RsyncCommandBuilder builder(info, this->m_config);
        QStringList commands = builder.build();

        foreach(QString command, commands) {
            qDebug() << command;
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

void SynchronizeJob::slot_start()
{
    QProcess *process = static_cast<QProcess*>(this->sender());
    QString dir = process->property("entry").toString();

    this->m_logger->log(QString("Synchronizing %1 dir...").arg(dir));

    RunningPayload *payload = new RunningPayload();

    payload->addDirInfo(dir, RunningPayload::Started);

    emit(running(payload));
}

void SynchronizeJob::slot_finished(int code)
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString dir = process->property("entry").toString();
    RunningPayload *payload = new RunningPayload();

    payload->addDirInfo(dir, code > 0 ? RunningPayload::Failed : RunningPayload::Finished);

    emit(running(payload));

    this->m_activeProcessList.remove(process->property("hash").toString());

    if(code > 0) {
        this->m_logger->error(QString("Synchronizing %1 dir failed").arg(dir));
    } else {
        this->m_logger->log(QString("Synchronizing %1 dir done").arg(process->property("entry").toString()));
    }

    if(this->m_activeProcessList.isEmpty()) {
        emit(finished(code));
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
