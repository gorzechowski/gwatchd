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

#include "sshsettingstest.h"

SshSettingsTest::SshSettingsTest(QObject *parent) :
    QObject(parent)
{

}

void SshSettingsTest::initTestCase()
{
    this->m_settings = new SshSettings(
                QStringList() << "host1" << "host2",
                "user",
                "/home/user/.ssh/key",
                "/etc/sshConfig",
                2222,
                QStringList() << "StrictHostKeyChecking=no"
    );
}

void SshSettingsTest::testValues()
{
    QCOMPARE(this->m_settings->hosts(), QStringList() << "host1" << "host2");
    QCOMPARE(this->m_settings->user(), QString("user"));
    QCOMPARE(this->m_settings->identityFile(), QString("/home/user/.ssh/key"));
    QCOMPARE(this->m_settings->configFile(), QString("/etc/sshConfig"));
    QCOMPARE(this->m_settings->port(), 2222);
    QCOMPARE(this->m_settings->options(), QStringList() << "StrictHostKeyChecking=no");
}
