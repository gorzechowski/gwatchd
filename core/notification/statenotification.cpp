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

#include <QJsonDocument>
#include <QJsonObject>

#include "statenotification.h"

StateNotification::StateNotification(QString jobName, int state, Payload *payload) : Notification(jobName)
{
    this->m_state = state;
    this->m_payload = payload;
}

QString StateNotification::getState()
{
    QString state;

    switch(this->m_state) {
        case StateNotification::Started:
            state = "started";
            break;

        case StateNotification::Running:
            state = "running";
            break;

        case StateNotification::Finished:
            state = "finished";
            break;

        case StateNotification::Failed:
            state = "failed";
            break;

        default:
            state = "unknown";
            break;
    }

    return state;
}

Payload* StateNotification::getPayload()
{
    return this->m_payload;
}

QString StateNotification::toJson()
{
    QJsonObject root;
    root.insert("job", this->getJobName());
    root.insert("state", this->getState());

    if(this->getPayload()) {
        root.insert("payload", this->getPayload()->toJsonObject());
    }

    QJsonDocument document(root);

    return QString(document.toJson(QJsonDocument::Compact));
}
