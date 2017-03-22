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

#ifndef RUNNINGPAYLOAD_H
#define RUNNINGPAYLOAD_H

#include <QtPlugin>

#include "../../core/notification/payload.h"

class RunningPayload: public Payload
{

    Q_INTERFACES(Payload)

public:
    RunningPayload();

    void addEntryInfo(QString entry, int state);

    enum State {
        Started = 0,
        Finished,
        Failed
    };

    QJsonObject toJsonObject();

protected:
    QHash<QString, QHash<QString, QString> > m_payload;

    QString stateToString(int state);
};

#endif // RUNNINGPAYLOAD_H
