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

#include "loggercomposite.h"

LoggerComposite::LoggerComposite()
{

}

void LoggerComposite::log(QString content)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->log(content);
    }
}

void LoggerComposite::debug(QString content)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->debug(content);
    }
}

void LoggerComposite::error(QString content)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->error(content);
    }
}

void LoggerComposite::setDebug(bool isDebug)
{
    foreach(Logger *logger, this->m_loggers) {
        logger->setDebug(isDebug);
    }
}

void LoggerComposite::add(Logger *logger)
{
    this->m_loggers << logger;
}
