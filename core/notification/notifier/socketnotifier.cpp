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

#include <QDebug>
#include <QNetworkInterface>

#include "notification/notifier/socketnotifier.h"

SocketNotifier::SocketNotifier(Config *config, Logger *logger, QObject *parent) : QObject(parent)
{
    this->m_config = config;
    this->m_logger = logger;

    this->startServer(
        this->m_config->value("tcp.listenAddress", "").toString(),
        this->m_config->value("tcp.listenPort", 0).toInt()
    );
}

bool SocketNotifier::startServer(QString address, int port)
{
    if(port > 0 && !address.isEmpty()) {
        this->m_server = new SocketServer(this->m_logger);

        return this->m_server->listen(address, port);
    }

    return false;
}

bool SocketNotifier::notify(Notification *notification)
{
    this->m_server->sendMessageToAllClients(notification->toJson());
}
