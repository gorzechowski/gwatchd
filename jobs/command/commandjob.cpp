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
#include <QRegularExpression>

#include "commandjob.h"
#include "command/ssh/sshcommandbuilder.h"
#include "notification/runningpayload.h"
#include "config/commandconfig.h"

CommandJob::CommandJob()
{
    this->m_timer = new QTimer();

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(slot_execute()));
}

void CommandJob::setConfig(Config *config)
{
    this->m_config = new CommandConfig(config);
}

void CommandJob::setLogger(Logger *logger)
{
    this->m_logger = logger;
}

QStringList CommandJob::getEntries()
{
    return this->m_config->entries();
}

void CommandJob::run(QString data)
{
    this->m_files << data;

    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    this->m_timer->start(this->m_config->value("delay").toInt(100));
}

void CommandJob::slot_execute()
{
    if(this->m_timer->isActive()) {
        this->m_timer->stop();
    }

    QStringList entries = this->retrieveEntries(this->m_files);

    this->m_files.clear();

    if(entries.isEmpty()) {
        this->m_logger->debug("Command job has nothing to do");

        return;
    }

    emit(started());

    foreach(QString entry, entries) {
        QFileInfo info(entry);

        QStringList commands;

        if(this->m_config->remote(entry)) {
            SshCommandBuilder builder(info, this->m_config);
            commands = builder.build();
        } else {
            commands << this->m_config->exec(entry);
        }

        foreach(QString command, commands) {
            QString hash = QCryptographicHash::hash(command.toUtf8(), QCryptographicHash::Md5).toHex();

            QProcess *process = new QProcess();

            process->setProcessChannelMode(QProcess::MergedChannels);

            if(this->m_activeProcessList.keys().contains(hash)) {
                this->m_logger->debug("Command already in progress " + hash);

                process = this->m_activeProcessList.value(hash);

                if(process->state() == QProcess::Running) {
                    this->m_logger->debug("Stopping command process");

                    disconnect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                    process->close();
                    connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));

                    this->m_logger->debug("Command process stopped");
                }
            } else {
                this->m_logger->debug("Creating new command process");

                connect(process, SIGNAL(started()), this, SLOT(slot_start()));
                connect(process, SIGNAL(finished(int)), this, SLOT(slot_finished(int)));
                connect(process, SIGNAL(readyRead()), this, SLOT(slot_read()));

                process->setProperty("entry", entry);
                process->setProperty("hash", hash);

                this->m_activeProcessList.insert(hash, process);

                this->m_logger->debug("New command process created");
            }

            this->m_logger->debug("Starting command process");

            process->start(command);
        }
    }
}

QStringList CommandJob::retrieveEntries(QStringList files)
{
    QStringList entries;

    foreach(QString entry, this->getEntries()) {
        foreach(QString file, files) {
            if(file.startsWith(entry)) {
                QString fileMask = this->m_config->fileMask(entry);

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

QString CommandJob::getCommand(QProcess *process)
{
    return QString("%1 %2").arg(process->program(), process->arguments().join(" "));
}

void CommandJob::slot_start()
{
    QProcess *process = static_cast<QProcess*>(this->sender());
    QString entry = process->property("entry").toString();
    QString command = this->getCommand(process);

    this->m_logger->log(QString("Running command: %1").arg(command));

    RunningPayload *payload = new RunningPayload();

    payload->addEntryInfo(entry, RunningPayload::Started);

    emit(running(payload));
}

void CommandJob::slot_finished(int code)
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString entry = process->property("entry").toString();
    QString command = this->getCommand(process);
    RunningPayload *payload = new RunningPayload();

    payload->addEntryInfo(entry, code > 0 ? RunningPayload::Failed : RunningPayload::Finished);

    emit(running(payload));

    this->m_activeProcessList.remove(process->property("hash").toString());

    if(code > 0) {
        this->m_logger->error(QString("Command %1 failed").arg(command));
    } else {
        this->m_logger->log(QString("Command %1 done").arg(command));
    }

    if(this->m_activeProcessList.isEmpty()) {
        emit(finished(code));
    }
}

void CommandJob::slot_read()
{
    QProcess *process = static_cast<QProcess*>(this->sender());

    QString data = QString(process->readAll()).trimmed().remove("\n");

    if(!data.isEmpty()) {
        this->m_logger->log(data);
    }
}
