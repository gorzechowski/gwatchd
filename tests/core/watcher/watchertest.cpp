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
#include <QSignalSpy>
#include <QEventLoop>
#include <QThread>
#include <QDebug>
#include <QProcess>

#include "watchertest.h"
#include "../../mockups/loggermock.h"

WatcherTest::WatcherTest(QObject *parent) :
    QObject(parent)
{
    this->m_dir = QDir("/tmp/gwatchd-test/");
}

void WatcherTest::initTestCase()
{
    if(this->m_dir.exists()) {
        this->m_dir.removeRecursively();
    }

    this->m_dir.mkpath("./dir1");
    this->m_dir.mkpath("./dir2");
    this->m_dir.mkpath("./dir1/dir1");
    this->m_dir.mkpath("./dir1/dir1/dir1");

    this->m_files.insert("/file1", "/file2");
    this->m_files.insert("/dir1/file1", "/file3");
    this->m_files.insert("/dir2/file1", "/dir1/file2");
    this->m_files.insert("/dir1/dir1/file1", "/dir1/dir1/file2");
    this->m_files.insert("/dir1/dir1/dir1/file1", "/dir1/dir1/dir1/file4");
    this->m_files.insert("/dir1/dir1/dir1/file2", "/dir1/dir1/file3");
    this->m_files.insert("/dir1/dir1/dir1/file3", "/dir2/file2");

    LoggerMock *logger = new LoggerMock();

    this->m_watcher = new Watcher(logger);

    this->m_watcher->addEntries(QStringList() << this->m_dir.absolutePath());

    QEventLoop loop;

    connect(this->m_watcher, SIGNAL(initialized()), &loop, SLOT(quit()));

    this->m_watcher->init();

    loop.exec();
}

void WatcherTest::cleanupTestCase()
{
    QProcess *process = new QProcess();

    process->start("rm", QStringList() << "-r" << this->m_dir.absolutePath());
}

void WatcherTest::testCreateFile()
{
    QThread::msleep(100);

    QSignalSpy spy(this->m_watcher, SIGNAL(fileChanged(QString)));

    QFile file;

    foreach(QString fileName, this->m_files.keys()) {
        file.setFileName(this->m_dir.absolutePath() + fileName);
        file.open(QIODevice::ReadWrite);

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        file.close();
    }
}

void WatcherTest::testModifyFile()
{
    QThread::msleep(100);

    QSignalSpy spy(this->m_watcher, SIGNAL(fileChanged(QString)));

    QFile file;

    foreach(QString fileName, this->m_files.keys()) {
        file.setFileName(this->m_dir.absolutePath() + fileName);
        file.open(QIODevice::ReadWrite);

        file.write("test");
        file.flush();

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        file.close();
    }
}

void WatcherTest::testCopyFile()
{
    QThread::msleep(100);

    QSignalSpy spy(this->m_watcher, SIGNAL(fileChanged(QString)));

    QFile file;

    foreach(QString key, this->m_files.keys()) {
        QString src = this->m_dir.absolutePath() + key;
        QString dest = this->m_dir.absolutePath() + this->m_files.value(key);

        file.setFileName(src);
        file.copy(src, dest);

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        QThread::msleep(100);

        file.setFileName(dest);

        file.open(QIODevice::ReadWrite);
        file.write("test");
        file.flush();

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        file.close();
    }
}

void WatcherTest::testMoveFile()
{
    QThread::msleep(100);

    QSignalSpy spy(this->m_watcher, SIGNAL(fileChanged(QString)));

    QFile file;

    foreach(QString fileName, this->m_files.values()) {
        QString src = this->m_dir.absolutePath() + fileName;
        QString dest = this->m_dir.absolutePath() + this->m_files.key(fileName);

        dest.replace("file", "moved");

        file.setFileName(src);
        file.rename(dest);

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        QThread::msleep(100);

        file.open(QIODevice::ReadWrite);
        file.write("test");
        file.flush();

        spy.wait(100);
        QCOMPARE(spy.count() > 0, true);

        spy.clear();

        file.close();
    }
}

#include "moc_watchertest.cpp"
