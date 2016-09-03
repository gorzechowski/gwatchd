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

#include "runningpayload.h"

RunningPayload::RunningPayload()
{

}

void RunningPayload::addEntryInfo(QString entry, int state)
{
    QHash<QString, QString> details;

    details.insert("state", QString::number(state));

    this->m_payload.insert(entry, details);
}

QJsonObject RunningPayload::toJsonObject()
{
    QJsonObject root;
    QJsonObject dirs;

    foreach(QString dir, this->m_payload.keys()) {
        QJsonObject details;

        details.insert("state", this->stateToString(this->m_payload.value(dir).value("state").toInt()));

        dirs.insert(dir, details);
    }

    root.insert("dirs", dirs);

    return root;
}

QString RunningPayload::stateToString(int state)
{
    QString result;

    switch(state) {
        case RunningPayload::Started:
            result = "started";
            break;

        case RunningPayload::Finished:
            result = "finished";
            break;

        case RunningPayload::Failed:
            result = "failed";
            break;

        default:
            result = "unknown";
            break;
    }

    return result;
}
