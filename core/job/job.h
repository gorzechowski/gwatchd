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

#ifndef JOB_H
#define JOB_H

#include <QtPlugin>

#include "config/config.h"
#include "logger/logger.h"
#include "notification/payload.h"

class Entry: public QString
{
public:
    Entry(QString string): QString(string) {}
    Entry() {}
};

class Predefine: public QString
{
public:
    Predefine(QString string): QString(string) {}
    Predefine() {}
};

class Job
{
public:
    virtual ~Job() {}

    virtual QStringList getEntries() { return this->m_config->value("entries").toObject().keys(); }
    virtual void run(Entry entry) = 0;
    virtual void run(Predefine predefine) = 0;
    virtual void setConfig(Config *config) { m_config = config; }
    virtual void setLogger(Logger *logger) { m_logger = logger; }

protected:
    Config *m_config;
    Logger *m_logger;

signals:
    void started();
    void running(Payload*);
    void finished(int);
    void runRequested(QString, Entry);
    void runRequested(QString, Predefine);
};

Q_DECLARE_INTERFACE(Job, "job/1.0")

#endif // JOB_H
