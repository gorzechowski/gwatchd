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

#ifndef RSYNCCOMMANDPARTEXCLUDESTEST_H
#define RSYNCCOMMANDPARTEXCLUDESTEST_H

#include <QObject>

#include "../../../jobs/synchronize/command/rsync/rsynccommandpartexcludes.h"
#include "config/synchronizeconfig.h"
#include "config/jsonconfig.h"

class RsyncCommandPartExcludesTest : public QObject
{
    Q_OBJECT
public:
    RsyncCommandPartExcludesTest(QObject *parent = 0);

private:
    SynchronizeConfig *m_config;
    RsyncCommandPartExcludes *m_commandPart;

private slots:
    void initTestCase();

    void testBuild();

};

#endif // RSYNCCOMMANDPARTEXCLUDESTEST_H
