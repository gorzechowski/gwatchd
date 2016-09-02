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

#ifndef JSONCONFIGTEST_H
#define JSONCONFIGTEST_H

#include <QObject>

#include "../../../core/config/jsonconfig.h"

class JsonConfigTest: public QObject
{
    Q_OBJECT
public:
    JsonConfigTest(QObject *parent = 0);

protected:
    JsonConfig *m_config;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testValue();
    void testListValue();
    void testDefaultValue();
    void testMissingFile();
};

#endif // JSONCONFIGTEST_H
