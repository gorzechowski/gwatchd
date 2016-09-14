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
#include <QProcess>
#include <QEventLoop>

#include "jobscollectortest.h"

#include "../../mockups/loggermock.h"
#include "job/jobscollector.h"

JobsCollectorTest::JobsCollectorTest()
{
    this->m_dir = QDir("/tmp/gwatchd-test/");
}

void JobsCollectorTest::initTestCase()
{
    if(this->m_dir.exists()) {
        this->m_dir.removeRecursively();
    }

    this->m_dir.mkpath("./config/job");
    this->m_dir.mkpath("./jobs");

    this->m_configsDirPath = QDir::cleanPath(this->m_dir.absolutePath() + "/config/job");
    this->m_jobsDirPath = QDir::cleanPath(this->m_dir.absolutePath() + "/jobs");
}

void JobsCollectorTest::init()
{
    this->initTestCase();
}

void JobsCollectorTest::cleanupTestCase()
{
    QProcess *process = new QProcess();

    QEventLoop loop;

    connect(process, SIGNAL(finished(int)), &loop, SLOT(quit()));

    process->start("rm", QStringList() << "-r" << this->m_dir.absolutePath());

    loop.exec();
}

void JobsCollectorTest::cleanup()
{
    this->cleanupTestCase();
}

void JobsCollectorTest::testCollectedJobs()
{
    LoggerMock *logger = new LoggerMock();

    QFile jobConfig(this->m_configsDirPath + "/first.json");
    QFile jobLib(this->m_jobsDirPath + "/libfirstjob.so");
    QFile secondJobConfig(this->m_configsDirPath + "/second.json");
    QFile secondJobLib(this->m_jobsDirPath + "/libsecondjob.so");

    if(jobConfig.open(QIODevice::ReadWrite) && jobLib.open(QIODevice::ReadWrite)) {
        jobConfig.close();
        jobLib.close();
    }

    if(secondJobConfig.open(QIODevice::ReadWrite) && secondJobLib.open(QIODevice::ReadWrite)) {
        secondJobConfig.close();
        secondJobLib.close();
    }

    JobsCollector *collector = new JobsCollector(
        this->m_configsDirPath,
        this->m_jobsDirPath,
        logger
    );

    QCOMPARE(collector->collectedJobs().count(), 2);
}

void JobsCollectorTest::testMissingConfigs()
{
    LoggerMock *logger = new LoggerMock();

    QFile jobLib(this->m_jobsDirPath + "/libfirstjob.so");
    QFile secondJobLib(this->m_jobsDirPath + "/libsecondjob.so");

    if(jobLib.open(QIODevice::ReadWrite) && secondJobLib.open(QIODevice::ReadWrite)) {
        jobLib.close();
        secondJobLib.close();
    }

    JobsCollector *collector = new JobsCollector(
        this->m_configsDirPath,
        this->m_jobsDirPath,
        logger
    );

    QCOMPARE(collector->collectedJobs().count(), 0);
}

void JobsCollectorTest::testMissingJobs()
{
    LoggerMock *logger = new LoggerMock();

    QFile jobConfig(this->m_configsDirPath + "/first.json");
    QFile secondJobConfig(this->m_configsDirPath + "/second.json");

    if(jobConfig.open(QIODevice::ReadWrite) && secondJobConfig.open(QIODevice::ReadWrite)) {
        jobConfig.close();
        secondJobConfig.close();
    }

    JobsCollector *collector = new JobsCollector(
        this->m_configsDirPath,
        this->m_jobsDirPath,
        logger
    );

    QCOMPARE(collector->collectedJobs().count(), 0);
}

void JobsCollectorTest::testMissingPair()
{
    LoggerMock *logger = new LoggerMock();

    QFile jobConfig(this->m_configsDirPath + "/first.json");
    QFile jobLib(this->m_jobsDirPath + "/libsomejob.so");
    QFile secondJobConfig(this->m_configsDirPath + "/second.json");
    QFile secondJobLib(this->m_jobsDirPath + "/libsecondjob.so");

    if(jobConfig.open(QIODevice::ReadWrite) && jobLib.open(QIODevice::ReadWrite)) {
        jobConfig.close();
        jobLib.close();
    }

    if(secondJobConfig.open(QIODevice::ReadWrite) && secondJobLib.open(QIODevice::ReadWrite)) {
        secondJobConfig.close();
        secondJobLib.close();
    }

    JobsCollector *collector = new JobsCollector(
        this->m_configsDirPath,
        this->m_jobsDirPath,
        logger
    );

    QCOMPARE(collector->collectedJobs().count(), 1);
}

#include "moc_jobscollectortest.cpp"
