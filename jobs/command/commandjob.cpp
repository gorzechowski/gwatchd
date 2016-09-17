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

#include "commandjob.h"
#include "command/ssh/sshcommandbuilder.h"
#include "notification/runningpayload.h"
#include "config/settings/factory/sshsettingsfactory.h"
#include "config/settings/factory/commandsettingsfactory.h"

CommandJob::CommandJob()
{
    this->m_entryTimer = new QTimer();
    this->m_predefineTimer = new QTimer();

    connect(this->m_entryTimer, SIGNAL(timeout()), this, SLOT(execute()));
    connect(this->m_predefineTimer, SIGNAL(timeout()), this, SLOT(execute()));
}

void CommandJob::setConfig(Config *config)
{
    this->m_config = config;
}

void CommandJob::setLogger(Logger *logger)
{
    this->m_logger = logger;
}

QStringList CommandJob::getEntries()
{
    return this->m_config->value("dirs").toObject().keys();
}

void CommandJob::run(Entry entry)
{
    if(this->m_entries.indexOf(entry) < 0) {
        this->m_entries << entry;
    }

    if(this->m_entryTimer->isActive()) {
        this->m_entryTimer->stop();
    }

    this->m_entryTimer->start(this->m_config->value("delay").toInt(100));
}

void CommandJob::run(Predefine predefine)
{
    if(this->m_predefines.indexOf(predefine) < 0) {
        this->m_predefines << predefine;
    }

    if(this->m_predefineTimer->isActive()) {
        this->m_predefineTimer->stop();
    }

    this->m_predefineTimer->start(this->m_config->value("delay").toInt(100));
}

void CommandJob::execute()
{
    QTimer *timer = dynamic_cast<QTimer*>(this->sender());

    if(timer == this->m_entryTimer) {
        this->execute(this->retrieveEntries(this->m_entries));
    } else if(timer == this->m_predefineTimer) {
        this->execute(this->m_predefines);
    }
}

void CommandJob::execute(QList<Entry> entries)
{
    if(this->m_entryTimer->isActive()) {
        this->m_entryTimer->stop();
    }

    this->m_entries.clear();

    if(entries.isEmpty()) {
        this->m_logger->debug("Command job has nothing to do");

        emit(finished(0));

        return;
    }

    emit(started());

    foreach(Entry entry, entries) {
        QStringList commands;

        SshSettings sshSettings = SshSettingsFactory::create(entry, this->m_config);
        CommandSettings commandSettings = CommandSettingsFactory::create(entry, this->m_config);

        if(commandSettings.remote()) {
            SshCommandBuilder builder(&sshSettings, commandSettings.exec());
            commands = builder.build();
        } else {
            commands << commandSettings.exec();
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

void CommandJob::execute(QList<Predefine> predefines)
{
    if(this->m_predefineTimer->isActive()) {
        this->m_predefineTimer->stop();
    }

    this->m_predefines.clear();

    if(predefines.isEmpty()) {
        this->m_logger->debug("Command job has nothing to do");

        emit(finished(0));

        return;
    }

    emit(started());

    foreach(Predefine predefine, predefines) {
        QStringList commands;

        SshSettings sshSettings = SshSettingsFactory::create(predefine, this->m_config);
        CommandSettings commandSettings = CommandSettingsFactory::create(predefine, this->m_config);

        if(commandSettings.remote()) {
            SshCommandBuilder builder(&sshSettings, commandSettings.exec());
            commands = builder.build();
        } else {
            commands << commandSettings.exec();
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

                process->setProperty("entry", predefine);
                process->setProperty("hash", hash);

                this->m_activeProcessList.insert(hash, process);

                this->m_logger->debug("New command process created");
            }

            this->m_logger->debug("Starting command process");

            process->start(command);
        }
    }
}

QList<Entry> CommandJob::retrieveEntries(QList<Entry> entries)
{
    QList<Entry> result;

    foreach(QString entry, this->getEntries()) {
        foreach(QString file, entries) {
            if(file.startsWith(entry)) {
                CommandSettings commandSettings = CommandSettingsFactory::create(Entry(entry), this->m_config);
                QString fileMask = commandSettings.fileMask();

                if(!fileMask.isEmpty()) {
                    QString fileName = file.split("/").last();
                    QRegularExpression regex(fileMask);
                    QRegularExpressionMatch match = regex.match(fileName);

                    if(!match.hasMatch()) {
                        continue;
                    }
                }

                result << Entry(entry);
                break;
            }
        }
    }

    return result;
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
