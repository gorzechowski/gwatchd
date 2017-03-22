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
    QTest::addColumn<int>("state");
    QTest::addColumn<QString>("expectedState");

    QTest::newRow("started") << "test" << (int) StateNotification::Started << "started";
    QTest::newRow("finished") << "test" << (int) StateNotification::Finished << "finished";
    QTest::newRow("failed") << "test" << (int) StateNotification::Failed << "failed";
}

void StateNotificationTest::testCreateInstance()
{
    QFETCH(QString, job);
    QFETCH(int, state);
    QFETCH(QString, expectedState);

    StateNotification *notification = new StateNotification(job, state);

    QJsonObject object;
    object.insert("job", job);
    object.insert("state", expectedState);

    QJsonDocument document(object);

    QCOMPARE(notification->toJson(), QString(document.toJson(QJsonDocument::Compact)));
}

#include "moc_statenotificationtest.cpp"
