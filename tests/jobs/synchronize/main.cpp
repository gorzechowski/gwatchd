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

#include <QCoreApplication>
#include <QTest>

#include "rsynccommandpartbasetest.h"
#include "rsynccommandpartexcludestest.h"
#include "rsynccommandpartincludestest.h"
#include "rsynccommandpartremoteshelltest.h"
#include "rsynccommandparttargettest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    RsyncCommandPartBaseTest baseTest;
    RsyncCommandPartExcludesTest excludesTest;
    RsyncCommandPartIncludesTest includesTest;
    RsyncCommandPartRemoteShellTest remoteShellTest;
    RsyncCommandPartTargetTest targetTest;

    int res = 0;

    res += QTest::qExec(&baseTest, argc, argv);
    res += QTest::qExec(&excludesTest, argc, argv);
    res += QTest::qExec(&includesTest, argc, argv);
    res += QTest::qExec(&remoteShellTest, argc, argv);
    res += QTest::qExec(&targetTest, argc, argv);

    return res;
}
