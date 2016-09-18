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

#include "hookssettingstest.h"

HooksSettingsTest::HooksSettingsTest(QObject *parent) :
    QObject(parent)
{

}

void HooksSettingsTest::initTestCase()
{
    this->m_settings = new HooksSettings(
        QList<HookDescriptor>() << HookDescriptor("command", Predefine("finished")) << HookDescriptor("synchronize", Predefine("finished2")),
        QList<HookDescriptor>() << HookDescriptor("command", Predefine("failed"))
    );
}

void HooksSettingsTest::testFinishedHooks()
{
    QCOMPARE(this->m_settings->finishedHooks().count(), 2);

    HookDescriptor hook = this->m_settings->finishedHooks().at(0);

    QCOMPARE(hook.jobName(), QString("command"));
    QCOMPARE(hook.predefine(), Predefine("finished"));

    hook = this->m_settings->finishedHooks().at(1);

    QCOMPARE(hook.jobName(), QString("synchronize"));
    QCOMPARE(hook.predefine(), Predefine("finished2"));
}

void HooksSettingsTest::testFailedHooks()
{
    QCOMPARE(this->m_settings->failedHooks().count(), 1);

    HookDescriptor hook = this->m_settings->failedHooks().at(0);

    QCOMPARE(hook.jobName(), QString("command"));
    QCOMPARE(hook.predefine(), Predefine("failed"));
}
