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

//#include <QPluginLoader>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QDebug>
#include <QEventLoop>
//#include <QCoreApplication>

#include "job/jobmanager.h"
#include "job/job.h"
#include "config/jsonconfig.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/loggercomposite.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "logger/decorator/loggerleveldecorator.h"
#include "notification/statenotification.h"
#include "notification/factory/statenotificationfactory.h"

JobManager::JobManager(bool isDebug, Logger *logger, ApplicationConfig *config, QObject *parent) :
    QObject(parent)
{
    this->m_isDebug = isDebug;
    this->m_logger = logger;
    this->m_config = config;
}

QString JobManager::getJobName(QObject *job)
{
    QJsonObject data = job->property("metaData").toJsonObject();

    return data.value("name").toString();
}

void JobManager::slot_jobStarted()
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Started job " + name);

    emit(notification(StateNotificationFactory::create(name)));
}

void JobManager::slot_jobRunning(Payload *payload)
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Still running job " + name);

    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), payload)));
}

void JobManager::slot_jobFinished(int code)
{
    QString name = this->getJobName(this->sender());

    this->m_logger->debug("Finished job " + name + " with code " + QString::number(code));

    bool success = code == 0 ? true : false;

    emit(notification(StateNotificationFactory::create(this->getJobName(this->sender()), success)));
}
