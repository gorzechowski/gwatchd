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
#include <QTextStream>
#include <QDir>
#include <stdio.h>

#include "logger/filelogger.h"

FileLogger::FileLogger(QString path, ApplicationConfig *config, QObject *parent) :
    QObject(parent)
{
    this->m_config = config;

    this->m_file = new QFile(path);

    QDir dir(QFileInfo(path).path());

    if(!dir.mkpath(".")) {
        printf(
            "Could not create log dir %s\n",
            qPrintable(dir.absolutePath())
        );
    }

    if(!this->m_file->isOpen() && !this->m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        printf(
            "Could not open log file %s: %s\n",
            qPrintable(path),
            qPrintable(this->m_file->errorString())
        );
    }
}

void FileLogger::log(QString content)
{
    if(!this->m_file->isOpen() || !this->m_file->isWritable()) return;

    qint64 size = this->m_file->size() + content.length() + 1;

    if(size >= (this->m_config->logsMaxFileSize() * 1024)) {
        this->m_file->copy(this->m_file->fileName() + QString::number(this->getIndex()));
        this->m_file->resize(0);
        this->m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }

    QTextStream stream(this->m_file);

    stream << content << endl;
}

int FileLogger::getIndex()
{
    int index = 1;

    QDir dir(this->m_file->fileName().left(this->m_file->fileName().lastIndexOf("/")));
    QString fileName = this->m_file->fileName().split("/").last();

    QStringList files = dir.entryList(QDir::Files | QDir::Readable, QDir::Name);

    if(files.empty()) {
        return index;
    }

    index = files.last().replace(fileName, "").toInt() + 1;

    return index;
}
