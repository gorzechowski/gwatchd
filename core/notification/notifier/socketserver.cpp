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

#include <QHostAddress>

#include "socketserver.h"

SocketServer::SocketServer(Logger *logger, QString serverName) :
    QWebSocketServer(serverName, QWebSocketServer::NonSecureMode)
{
    this->m_logger = logger;

    connect(this, SIGNAL(newConnection()), this, SLOT(slot_addClient()));
}

bool SocketServer::listen(QString addr, int port)
{
    QHostAddress address;
    address.setAddress(addr);

    bool status = QWebSocketServer::listen(address, port);

    if(!status) {
        this->m_logger->log("Could not create websocket server: " + this->errorString());
    }

    return status;
}

void SocketServer::sendMessageToAllClients(QString message)
{
    foreach(QWebSocket *client, this->m_clients) {
        if(client->isValid()) {
            client->sendTextMessage(message);
        }
    }
}

void SocketServer::slot_addClient()
{
    QWebSocket *client = this->nextPendingConnection();

    this->m_clients.append(client);

    client->setProperty("index", this->m_clients.count() - 1);

    connect(client, SIGNAL(disconnected()), this, SLOT(slot_removeClient()));
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));
}

void SocketServer::slot_removeClient()
{
    QWebSocket *client = dynamic_cast<QWebSocket*>(this->sender());

    this->m_clients.removeAt(client->property("index").toInt());
}
