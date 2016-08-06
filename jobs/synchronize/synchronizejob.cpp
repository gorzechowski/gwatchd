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

#include "synchronizejob.h"
#include "command/rsync/rsynccommandbuilder.h"
#include "notification/runningpayload.h"

SynchronizeJob::SynchronizeJob()
{
    this->m_timer = new QTimer();

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(slot_synchronize()));
}

void SynchronizeJob::setConfig(Config *config)
{
    this->m_config = config;
}

void SynchronizeJob::setLogger(Logger *logger)
{
    this->m_logger = logger;
}

QStringList SynchronizeJob::getDirs()
{
    return this->m_config->listValue("dirs");
}

void SynchronizeJob::run(QString data)
{
    this->m_files << data;

    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    this->m_timer->start(this->m_config->value("delay", 100).toInt());
}

void SynchronizeJob::slot_synchronize()
{
    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    emit(started());

    QStringList dirs;

    foreach(QString dir, this->getDirs()) {
        foreach(QString file, this->m_files) {
            if(file.startsWith(dir)) {
                dirs << dir;
                break;
            }
        }
    }

    this->m_files.clear();

    foreach(QString dir, dirs) {
        RsyncCommandBuilder builder(dir, this->m_config);
        QStringList commands = builder.build();

        foreach(QString command, commands) {
            QString hash = QCryptographicHash::hash(command.toUtf8(), QCryptographicHash::Md5).toHex();

            QProcess *process = new QProcess();

            process->setProcessChannelMode(QProcess::MergedChannels);

            if(this->m_activeProcessList.keys().contains(hash)) {
                process = this->m_activeProcessList.value(hash);

                if(process->state() == QProcess::Running) {
                    disconnect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                    process->close();
                    connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                }
            } else {
                connect(process, SIGNAL(started()), this, SLOT(slot_start()));
                connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                connect(process, SIGNAL(readyRead()), this, SLOT(slot_read()));

                process->setProperty("dir", dir);
                process->setProperty("hash", hash);

                this->m_activeProcessList.insert(hash, process);
            }

            process->start(command);
        }
    }
}

void SynchronizeJob::slot_start()
{
    QProcess *process = static_cast<QProcess*>(this->sender());
    QString dir = process->property("dir").toString();

    this->m_logger->log(QString("Synchronizing %1 dir...").arg(dir));

    RunningPayload *payload = new RunningPayload();

    payload->addDirInfo(dir, RunningPayload::Started);

    emit(running(payload));
}

void SynchronizeJob::slot_finished(int code)
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString dir = process->property("dir").toString();
    QString error = QString(process->readAllStandardError()).trimmed().remove("\n");
    RunningPayload *payload = new RunningPayload();
    int status;

    if(code > 0) {
        this->m_logger->log(QString("Synchronizing %1 dir failed: %2").arg(dir).arg(error));

        status = RunningPayload::Failed;
    } else {
        this->m_logger->log(QString("Synchronizing %1 dir done").arg(process->property("dir").toString()));

        status = RunningPayload::Finished;
    }

    payload->addDirInfo(dir, status);

    emit(running(payload));

    this->m_activeProcessList.remove(process->property("hash").toString());

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
