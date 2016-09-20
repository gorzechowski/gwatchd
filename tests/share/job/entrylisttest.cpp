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

#include "entrylisttest.h"

EntryListTest::EntryListTest(QObject *parent) :
    QObject(parent)
{

}

void EntryListTest::initTestCase()
{
    this->m_config = new JsonConfig(":/command.json");
    this->m_list << Entry("/dir3/test.txt") << Entry("/dir3/src/file.php") << Entry("/dir3/config/autoload/local.conf") << Entry("/dir3/some/dir/path/localFile");
}

void EntryListTest::testFilterEntries()
{
    EntryList filtered = this->m_list.filterEntries(QStringList() << "/dir3/", this->m_config);

    QCOMPARE(filtered.count(), 1);
    QCOMPARE(filtered.at(0), Entry("/dir3/config/autoload/local.conf"));
}
