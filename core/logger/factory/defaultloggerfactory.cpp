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

#include "logger/loggercomposite.h"
#include "logger/filelogger.h"
#include "logger/simplelogger.h"
#include "logger/decorator/loggertimestampdecorator.h"
#include "logger/decorator/loggerleveldecorator.h"

#include "defaultloggerfactory.h"

DefaultLoggerFactory::DefaultLoggerFactory()
{

}

Logger* DefaultLoggerFactory::create(QString filePath, ApplicationConfig *config, bool isDebug)
{
    LoggerLevelDecorator *fileLogger = new LoggerLevelDecorator(
        new LoggerTimestampDecorator(
            new FileLogger(filePath, config)
        )
    );

    LoggerLevelDecorator *simpleLogger = new LoggerLevelDecorator(
        new LoggerTimestampDecorator(
            new SimpleLogger()
        )
    );

    LoggerComposite *logger = new LoggerComposite();

    logger->add(fileLogger);
    logger->add(simpleLogger);

    logger->setDebug(isDebug);

    return logger;
}
