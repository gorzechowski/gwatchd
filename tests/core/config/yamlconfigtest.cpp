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
#include <QDebug>
#include "yamlconfigtest.h"

YamlConfigTest::YamlConfigTest(QObject *parent) :
    QObject(parent)
{
}

void YamlConfigTest::initTestCase()
{
    this->m_config = new YamlConfig(":/synchronize.yml");
}

void YamlConfigTest::cleanupTestCase()
{

}

void YamlConfigTest::testValue()
{
    QCOMPARE(this->m_config->value("delay").toInt(), 100);

    QCOMPARE(this->m_config->value("ssh.identityFile").toString(), QString("/home/user/.ssh/id_rsa"));

    QCOMPARE(this->m_config->value("ssh.port").toInt(), 22);
    QCOMPARE(this->m_config->value("dirs./dir2/.ssh.port").toInt(), 2222);

    QCOMPARE(this->m_config->value("target.user").toString(), QString("user"));
    QCOMPARE(this->m_config->value("dirs./dir2/.target.user").toString(), QString("otherUser"));

    QCOMPARE(this->m_config->value("dirs./dir1/.target.path").toString(), QString("/backup/dir1/"));
    QCOMPARE(this->m_config->value("dirs./dir2/.target.path").toString(), QString("/backup/dir2/"));
}

void YamlConfigTest::testListValue()
{
    QCOMPARE(
        this->m_config->listValue("dirs").contains("/dir1/") && this->m_config->listValue("dirs").contains("/dir2/"),
        true
    );

    QCOMPARE(
        this->m_config->listValue("dirs./dir1/.excludes"),
        QStringList() << "*.git" << "*.local"
    );

    QCOMPARE(
        this->m_config->listValue("dirs./dir2/.excludes"),
        QStringList()
    );

    QCOMPARE(
        this->m_config->listValue("dirs./dir1/.includes"),
        QStringList() << "important.local" << "important2.local"
    );

    QCOMPARE(
        this->m_config->listValue("dirs./dir2/.includes"),
        QStringList()
    );

    QCOMPARE(
        this->m_config->listValue("target.hosts"),
        QStringList() << "127.0.0.1"
    );

    QCOMPARE(
        this->m_config->listValue("ssh.options"),
        QStringList() << "StrictHostKeyChecking=no"
    );
}

void YamlConfigTest::testDefaultValue()
{
    QCOMPARE(this->m_config->value("undefined", "default").toString(), QString("default"));

    QCOMPARE(
        this->m_config->listValue("undefined.list", QStringList() << "item1"),
        QStringList() << "item1"
    );
}

void YamlConfigTest::testMissingFile()
{
    YamlConfig *config = new YamlConfig("/not/existing/file");

    QCOMPARE(config->value("undefined", "default").toString(), QString("default"));

    QCOMPARE(
        this->m_config->listValue("undefined.list", QStringList() << "item1"),
        QStringList() << "item1"
    );
}
