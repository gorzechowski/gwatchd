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
#include <QFileInfo>

#include "config/jsonconfig.h"
#include "config/settings/factory/sshsettingsfactory.h"

#include "sshcommandparttargettest.h"

SshCommandPartTargetTest::SshCommandPartTargetTest(QObject *parent) :
    QObject(parent)
{
}

void SshCommandPartTargetTest::testBuildNoUser()
{
    SshSettings sshSettings = SshSettingsFactory::create(Entry("/dir2/file.txt"), new JsonConfig(":/command.json"));

    SshCommandPartTarget *part = new SshCommandPartTarget(&sshSettings);

    QStringList hosts = sshSettings.hosts();

    QCOMPARE(hosts.count(), 1);

    foreach(QString host, hosts) {
        QCOMPARE(part->build(host), QString("example.com"));
    }
}

void SshCommandPartTargetTest::testBuild()
{
    SshSettings sshSettings = SshSettingsFactory::create(Entry("/dir1/"), new JsonConfig(":/command.json"));

    SshCommandPartTarget *part = new SshCommandPartTarget(&sshSettings);

    QStringList hosts = sshSettings.hosts();

    QCOMPARE(hosts.count(), 1);

    foreach(QString host, hosts) {
        QCOMPARE(part->build(host), QString("user@example.com"));
    }
}
