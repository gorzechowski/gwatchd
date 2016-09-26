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

#include <QRegularExpression>

#include "config/settings/factory/settingsfactory.h"

#include "entrylist.h"

EntryList::EntryList()
{

}

EntryList EntryList::filterEntries(QStringList list, Config *config)
{
    EntryList result;

    foreach(Entry value, list) {
        foreach(Entry entry, *this) {
            if(entry.startsWith(value)) {
                QFileInfo info(entry);

                Settings settings = SettingsFactory::create(value, config);
                QString fileMask = settings.fileMask();

                if(!fileMask.isEmpty() && (info.isFile() || info.fileName().contains(QRegularExpression(".*\\..*")))) {
                    QString fileName = entry.split("/").last();
                    QRegularExpression regex(fileMask);
                    QRegularExpressionMatch match = regex.match(fileName);

                    if(!match.hasMatch()) {
                        continue;
                    }
                }

                result << value;
                break;
            }
        }
    }

    return result;
}

EntryList EntryList::filter(QRegularExpression regex)
{
    EntryList result;

    foreach(Entry entry, *this) {
        QFileInfo info(entry);

        if(info.isFile() || info.fileName().contains(QRegularExpression(".*\\..*"))) {
            QString fileName = entry.split("/").last();
            QRegularExpressionMatch match = regex.match(fileName);

            if(!match.hasMatch()) {
                continue;
            }
        }

        result << entry;
        break;
    }

    return result;
}
