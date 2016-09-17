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

#include "jsonconfigtest.h"

JsonConfigTest::JsonConfigTest(QObject *parent) : QObject(parent)
{

}

void JsonConfigTest::initTestCase()
{
    this->m_config = new JsonConfig(":/synchronize.json");
}

void JsonConfigTest::cleanupTestCase()
{

}

void JsonConfigTest::testValue()
{
    QCOMPARE(this->m_config->value("delay").toInt(), 100);

    QCOMPARE(this->m_config->value("ssh").toObject().value("identityFile").toString(), QString("/home/user/.ssh/id_rsa"));

    QCOMPARE(this->m_config->value("ssh").toObject().value("port").toInt(), 22);
    QCOMPARE(this->m_config->value("dirs").toObject().value("/dir2/").toObject().value("ssh").toObject().value("port").toInt(), 2222);

    QCOMPARE(this->m_config->value("target").toObject().value("user").toString(), QString("user"));
    QCOMPARE(this->m_config->value("dirs").toObject().value("/dir2/").toObject().value("target").toObject().value("user").toString(), QString("otherUser"));
}
