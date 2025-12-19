/*
 *  Copyright (C) 2025 Procion ByProcion@gmail.com
 *
 *  This file is part of Wirus Mod Manager.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 *
 */
#ifndef WINROOTBACKEND_H
#define WINROOTBACKEND_H

#include <QStringList>
#include <QLocalServer>
#include <QApplication>
#include <filesystem>
#include "../core/InterprocessData.h"

class winrootbackend : public QObject
{
    Q_OBJECT
    QLocalServer server;
    QLocalSocket* client = nullptr;
    QString token;

    void distributor(ComandList func, QByteArray& data);
    void dir_comparison(QByteArray& data);
    void dir_comparison(const std::filesystem::path& file,
                        const std::string& core_dir_name,
                        const std::string& CONFIG_GAME_PATH,
                        const std::string& CONFIG_GAME,
                        const QStringList& MGD);
    void symlink_deliting(QByteArray& data);
    void symlink_creating(QByteArray& data);
    void kill();

private slots:
    void connection();

public:
    winrootbackend(const QString& pipe, const QString& token);
};

#endif // WINROOTBACKEND_H
