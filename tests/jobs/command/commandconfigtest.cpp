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

#include "commandconfigtest.h"

CommandConfigTest::CommandConfigTest(QObject *parent) : QObject(parent)
{

}

void CommandConfigTest::initTestCase()
{
    this->m_config = new CommandConfig(new JsonConfig(":/command.json"));
}

void CommandConfigTest::testValue()
{
    QCOMPARE(this->m_config->remote("/dir3/"), true);
    QCOMPARE(this->m_config->exec("/dir3/"), QString("./script"));
    QCOMPARE(this->m_config->fileMask("/dir3/"), QString("local.*"));
    QCOMPARE(this->m_config->sshUser("/dir1/"), QString("user"));
    QCOMPARE(this->m_config->sshIdentityFile("/dir1/"), QString("/home/user/.ssh/id_rsa"));
    QCOMPARE(this->m_config->sshIdentityFile("/dir2/file.txt"), QString("/home/user/.ssh/key"));
    QCOMPARE(this->m_config->sshConfigFile("/dir1/"), QString("/home/user/sshConfig"));
    QCOMPARE(this->m_config->sshConfigFile("/dir2/file.txt"), QString("/etc/sshConfig"));
    QCOMPARE(this->m_config->sshPort("/dir1/"), 22);
    QCOMPARE(this->m_config->sshPort("/dir2/file.txt"), 2222);
}

void CommandConfigTest::testListValue()
{
    QCOMPARE(
        this->m_config->entries(),
        QStringList() << "/dir1/" << "/dir2/file.txt" << "/dir3/"
    );

    QCOMPARE(
        this->m_config->sshHosts(),
        QStringList() << "example.com"
    );

    QCOMPARE(
        this->m_config->sshOptions(),
        QStringList() << "StrictHostKeyChecking=no"
    );
}

void CommandConfigTest::testMissingFile()
{
    CommandConfig *config = new CommandConfig(new JsonConfig("/not/existing/file"));

    QCOMPARE(config->value("undefined").toString("default"), QString("default"));
}
