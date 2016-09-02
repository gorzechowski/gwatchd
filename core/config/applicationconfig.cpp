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

#include "applicationconfig.h"

ApplicationConfig::ApplicationConfig(Config *config, QObject *parent) : QObject(parent)
{
    this->m_config = config;
}

QFileInfo ApplicationConfig::fileInfo()
{
    return this->m_config->fileInfo();
}

QString ApplicationConfig::logsDirPath()
{
    return this->m_config->value("log").toObject().value("dirPath").toString("logs");
}

int ApplicationConfig::logsMaxFileSize()
{
    return this->m_config->value("log").toObject().value("maxFileSize").toInt(5);
}

QString ApplicationConfig::socketAddress()
{
    return this->m_config->value("socket").toObject().value("address").toString("");
}

int ApplicationConfig::socketPort()
{
    return this->m_config->value("socket").toObject().value("port").toInt(0);
}
