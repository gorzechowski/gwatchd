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
#include <QDebug>

#include "socketserver.h"

SocketServer::SocketServer(Config *config, Logger *logger, QString serverName) :
    QWebSocketServer(serverName, QWebSocketServer::NonSecureMode)
{
    this->m_config = config;
    this->m_logger = logger;

    connect(this, SIGNAL(newConnection()), this, SLOT(slot_addClient()));
}

bool SocketServer::start()
{
    QString addr = this->m_config->value("socket").toObject(QJsonObject{{"address", ""}}).value("address").toString();
    int port = this->m_config->value("socket").toObject(QJsonObject{{"port", 0}}).value("port").toInt();

    if(port > 0 && !addr.isEmpty()) {
        QHostAddress address;
        address.setAddress(addr);

        this->m_logger->debug("Initializing Websocket server");

        bool status = QWebSocketServer::listen(address, port);

        if(status) {
            this->m_logger->log(tr("Websocket listening on %1:%2").arg(addr).arg(port));
        } else {
            this->m_logger->error("Could not create websocket server: " + this->errorString());
        }

        return status;
    } else {
        this->m_logger->debug("Websocket's port or address not specified");
    }

    return false;
}

void SocketServer::sendMessageToAllClients(QString message)
{
    foreach(QWebSocket *client, this->m_clients) {
        if(client->isValid()) {
            this->m_logger->debug("Sending Websocket message to client");
            client->sendTextMessage(message);
        } else {
            this->m_logger->debug("Websocket client is not valid");
        }
    }
}

void SocketServer::slot_addClient()
{
    this->m_logger->debug("Adding new Websocket client");

    QWebSocket *client = this->nextPendingConnection();

    this->m_clients.append(client);

    client->setProperty("index", this->m_clients.count() - 1);

    connect(client, SIGNAL(disconnected()), this, SLOT(slot_removeClient()));
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));

    this->m_logger->debug("Websocket client added");
}

void SocketServer::slot_removeClient()
{
    this->m_logger->debug("Removing Websocket client");

    QWebSocket *client = dynamic_cast<QWebSocket*>(this->sender());

    this->m_clients.removeAt(client->property("index").toInt());

    this->m_logger->debug("Websocket client removed");
}
