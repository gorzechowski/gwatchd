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

#include <QTest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "statenotificationtest.h"

StateNotificationTest::StateNotificationTest()
{

}

void StateNotificationTest::testCreateInstance_data()
{
    QTest::addColumn<QString>("job");
    QTest::addColumn<int>("status");
    QTest::addColumn<QString>("expectedStatus");

    QTest::newRow("started") << "test" << (int) StateNotification::Started << "started";
    QTest::newRow("finished") << "test" << (int) StateNotification::Finished << "finished";
    QTest::newRow("failed") << "test" << (int) StateNotification::Failed << "failed";
}

void StateNotificationTest::testCreateInstance()
{
    QFETCH(QString, job);
    QFETCH(int, status);
    QFETCH(QString, expectedStatus);

    StateNotification *notification = new StateNotification(job, status);

    QJsonObject object;
    object.insert("job", job);
    object.insert("status", expectedStatus);

    QJsonDocument document(object);

    QCOMPARE(notification->toJson(), QString(document.toJson()));
}

#include "moc_statenotificationtest.cpp"
