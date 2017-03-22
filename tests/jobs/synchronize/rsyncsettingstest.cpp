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
#include <QStringList>

#include "config/jsonconfig.h"

#include "rsyncsettingstest.h"

RsyncSettingsTest::RsyncSettingsTest(QObject *parent) :
    QObject(parent)
{

}

void RsyncSettingsTest::initTestCase()
{
    this->m_settings = new RsyncSettings(
        "/some/dir",
        QStringList() << "*.sh",
        QStringList(),
        QStringList() << "host1" << "host2",
        "/some/remote/dir",
        "user1"
    );
}

void RsyncSettingsTest::testValues()
{
    QCOMPARE(this->m_settings->source(), QString("/some/dir"));
    QCOMPARE(this->m_settings->excludes(), QStringList() << "*.sh");
    QCOMPARE(this->m_settings->includes(), QStringList());
    QCOMPARE(this->m_settings->targetHosts(), QStringList() << "host1" << "host2");
    QCOMPARE(this->m_settings->targetPath(), QString("/some/remote/dir"));
    QCOMPARE(this->m_settings->targetUser(), QString("user1"));
}
