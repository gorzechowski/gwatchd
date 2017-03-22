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

#include <QFile>

#include "jsonconfig.h"

JsonConfig::JsonConfig(QString filePath, QObject *parent) : Config(filePath, parent)
{
    QFile file(filePath);

    if(file.exists()) {
        file.open(QIODevice::ReadOnly);

        this->m_main = QJsonDocument::fromJson(file.readAll()).object();
    } else {
        this->m_main = QJsonObject();
    }
}

void JsonConfig::setLogger(Logger *logger)
{
    this->m_logger = logger;
}

QJsonValue JsonConfig::value(QString key)
{
    if(this->m_deprecated.keys().contains(key) && this->m_logger) {
        this->logDeprecated(key, this->m_deprecated.value(key));
    }

    if(this->m_deprecated.values().contains(key)) {
        if(!this->m_main.value(key).isObject()) {
            QString deprecated = this->m_deprecated.key(key);

            this->logDeprecated(deprecated, key);

            key = deprecated;
        }
    }

    return this->m_main.value(key);
}

void JsonConfig::logDeprecated(QString deprecated, QString replacement)
{
    QString log = QString("Key \"%1\" in \"%2\" is deprecated").arg(deprecated, this->fileInfo().fileName());

    if(!replacement.isEmpty()) {
        log.append(QString(". Please use \"%1\" instead").arg(replacement));
    }

    this->m_logger->warning(log);
}
