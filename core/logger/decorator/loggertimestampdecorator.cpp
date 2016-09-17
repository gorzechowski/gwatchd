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

#include <QDateTime>

#include "loggertimestampdecorator.h"

LoggerTimestampDecorator::LoggerTimestampDecorator(Logger *logger, QObject *parent) :
    QObject(parent)
{
    this->m_logger = logger;
}

void LoggerTimestampDecorator::log(QString content)
{
    content.prepend(this->timestamp());

    this->m_logger->log(content);
}

void LoggerTimestampDecorator::debug(QString content)
{
    content.prepend(this->timestamp());

    this->m_logger->debug(content);
}

void LoggerTimestampDecorator::error(QString content)
{
    content.prepend(this->timestamp());

    this->m_logger->error(content);
}

QString LoggerTimestampDecorator::timestamp()
{
    return QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
}
