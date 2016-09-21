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

#ifndef HOOKSSETTINGSFACTORY_H
#define HOOKSSETTINGSFACTORY_H

#include <QString>

#include "../hookssettings.h"
#include "../hookdescriptor.h"
#include "config/config.h"
#include "job/job.h"

class HooksSettingsFactory
{
public:
    HooksSettingsFactory(QString context, Config *config);

    static HooksSettings create(Entry entry, Config *config);
    static HooksSettings create(Predefine predefine, Config *config);

    HooksSettings create(QString entry);

protected:
    QString m_context;
    Config *m_config;

    QList<HookDescriptor> finishedHooks(QString entry);
    QList<HookDescriptor> failedHooks(QString entry);
    QList<HookDescriptor> hooks(QString type, QString entry);
};

#endif // HOOKSSETTINGSFACTORY_H
