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

#include "config/yamlconfig.h"

YamlConfig::YamlConfig(QString filePath, QObject *parent) :
    Config(filePath, parent)
{
    QFile file(filePath);

    if(file.exists()) {
        file.open(QIODevice::ReadOnly);

        this->m_config = YAML::Load(QString(file.readAll()).toStdString());
    } else {
        this->m_config = YAML::Load("{}");
    }
}

QVariant YamlConfig::value(QString key, QVariant defaultValue)
{
    QStringList keys = key.split(".");

    YAML::Node node = YAML::Clone(this->m_config);

    foreach(QString key, keys) {
        if(node[key.toStdString()]) {
            node = node[key.toStdString()];
        } else {
            return defaultValue;
        }
    }

    return QVariant(QString::fromStdString(node.as<std::string>()));
}

QStringList YamlConfig::listValue(QString key, QStringList defaultValue)
{
    QStringList keys = key.split(".");
    QStringList result;

    YAML::Node node = YAML::Clone(this->m_config);

    foreach(QString key, keys) {
        if(node[key.toStdString()]) {
            node = node[key.toStdString()];
        } else {
            return defaultValue;
        }
    }

    for(YAML::Node::const_iterator it = node.begin(); it != node.end(); it++) {
        YAML::Node item;

        if(node.IsMap()) {
            item = it->first;
        } else {
            item = node[result.count()];
        }

        result << QString::fromStdString(item.as<std::string>());
    }

    return result;
}
