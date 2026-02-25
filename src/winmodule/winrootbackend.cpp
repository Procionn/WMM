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
#include "winrootbackend.h"
#include "../CONSTANTS.h"
#include "methods.h"
#include <iostream>
#include <QLocalSocket>
#include <QTimer>
#include <wmml.h>
#include <windows.h>
#include <sys/stat.h>


namespace fs = std::filesystem;

winrootbackend::winrootbackend(const QString& pipe, const QString& newToken)
    : token(newToken) {
    server.setSocketOptions(QLocalServer::UserAccessOption);
    if (!server.listen(pipe))
        QCoreApplication::exit(1);

    connect(&server, &QLocalServer::newConnection, this, &winrootbackend::connection);

    QTimer::singleShot(1500, []{
        stc::cerr("auto-exit");
        QCoreApplication::exit(0);
    });
}


void winrootbackend::connection () {
    if (client) {
        server.nextPendingConnection()->close();
        return;
    }
    client = server.nextPendingConnection();
    connect(client, &QLocalSocket::readyRead, [this]{
        QDataStream pack(client);
        IpcHeader ipc;
        if (client->bytesAvailable() < sizeof(IpcHeader)) {
            stc::cerr("data lose"s);
            return;
        }

        pack.readRawData(reinterpret_cast<char*>(&ipc), sizeof(ipc));
        if (ipc.hash != IpcHeader::hash || ipc.version != IpcHeader::version) {
            stc::cerr(ipc.hash +" "s + IpcHeader::hash + "\n"s
                      + ipc.version + " "s + IpcHeader::version);
            kill();
        }
        if (client->bytesAvailable() < ipc.dataSize) {
            stc::cerr("data lose");
            return;
        }

        QByteArray data(ipc.dataSize, Qt::Uninitialized);
        QDataStream dataView(&data, QIODevice::ReadOnly);
        pack.readRawData(data.data(), data.size());
        QString receivedToken;
        dataView >> receivedToken;
        if (receivedToken != token)
            kill();
        QByteArray parameters;
        dataView >> parameters;

        distributor(ipc.comand, parameters);
    });
    connect(client, &QLocalSocket::disconnected, []{QCoreApplication::exit(0);});
}


void winrootbackend::distributor (ComandList func, QByteArray& data) {
    switch(func) {
    case ComandList::dir_cmp:
        dir_comparison(data);
        break;
    case ComandList::symlnk_create:
        symlink_creating(data);
        break;
    case ComandList::symlnk_del:
        symlink_deliting(data);
        break;
    case ComandList::kill:
    default:
        kill();
        break;
    }
}


void winrootbackend::dir_comparison (QByteArray& data) {
    QDataStream dataView(&data, QIODevice::ReadOnly);
    QStringList list;
    QString Qfile, Qcore_dir_name, QCONFIG_GAME_PATH, QCONFIG_GAME;
    std::string file, core_dir_name, CONFIG_GAME_PATH, CONFIG_GAME;
    dataView >> Qfile;
    dataView >> QCONFIG_GAME_PATH;
    dataView >> Qcore_dir_name;
    dataView >> QCONFIG_GAME;
    dataView >> list;
    file = Qfile.toStdString();
    core_dir_name = Qcore_dir_name.toStdString();
    CONFIG_GAME_PATH = QCONFIG_GAME_PATH.toStdString();
    CONFIG_GAME = QCONFIG_GAME.toStdString();
    dir_comparison(file, core_dir_name, CONFIG_GAME_PATH, CONFIG_GAME, list);
}

void winrootbackend::dir_comparison (const fs::path& file, const std::string& core_dir_name,
                                     const std::string& CONFIG_GAME_PATH, const std::string& CONFIG_GAME,
                                     const QStringList& MGD) {
    wmml targetFile(file);
    std::vector<wmml::variant> v(GRID_WIDTH);
    while(targetFile.read(v))
        if (std::get<std::string>(v[1]) == "this")
            break;
    try {
        fs::path pathToBackupPath = GAME / fs::path(core_dir_name);
        fs::path mixedGameDirectory;
        for (const auto& directory : MGD) { // std::string vector changed to QStringList
            mixedGameDirectory = CONFIG_GAME_PATH / fs::path(directory.toStdString());
            fs::path relative, pathToBackupFile, collectionFile;
            for (const auto& entry : fs::recursive_directory_iterator(mixedGameDirectory)) {
                relative = fs::relative(entry.path(), CONFIG_GAME_PATH);
                pathToBackupFile = pathToBackupPath / relative;
                collectionFile = (COLLECTIONS + CONFIG_GAME + "/" +
                                  std::get<std::string>(v[0])) / relative;
                if (!fs::exists(pathToBackupFile)) {
                    auto is_symlink = [](const std::filesystem::path& p) -> bool {
                        DWORD attrs = GetFileAttributesA(p.string().c_str());
                        return (attrs != INVALID_FILE_ATTRIBUTES) &&
                               (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
                    };

                    if (is_symlink(entry.path()))
                        continue;
                    struct stat info;
                    if (stat(entry.path().string().c_str(), &info) == 0 &&
                        S_ISDIR(info.st_mode))
                        continue;

                    fs::create_directories(collectionFile.parent_path());
                    if (fs::exists(collectionFile))
                        fs::remove(collectionFile);
                    fs::rename(stc::string::replace(entry.path(), '\\', '/'),
                               stc::string::replace(collectionFile, '\\', '/'));
                }
            }
        }
    }
    catch (const std::exception& e) {
        stc::cerr("Error: "s + e.what());
    }
}


void winrootbackend::symlink_deliting (QByteArray& data) {
    QDataStream dataView(&data, QIODevice::ReadOnly);
    QStringList list;
    QString Qcore_dir_name, QCONFIG_GAME_PATH, QCONFIG_GAME;
    std::string core_dir_name, CONFIG_GAME_PATH, CONFIG_GAME;
    dataView >> QCONFIG_GAME_PATH;
    dataView >> Qcore_dir_name;
    dataView >> QCONFIG_GAME;
    dataView >> list;
    core_dir_name = Qcore_dir_name.toStdString();
    CONFIG_GAME_PATH = QCONFIG_GAME_PATH.toStdString();
    CONFIG_GAME = QCONFIG_GAME.toStdString();

    fs::path testFile = (CONFIG_GAME_PATH + "/" + CONST_FILE + EXPANSION);
    if (fs::exists(testFile))
        dir_comparison(testFile, core_dir_name, CONFIG_GAME_PATH, CONFIG_GAME, list);
    try {
        auto is_symlink = [](const std::filesystem::path& p) -> bool {
            DWORD attrs = GetFileAttributesA(p.string().c_str());
            return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
        };

        for (const auto& entry : fs::recursive_directory_iterator(CONFIG_GAME_PATH)) {
            if (is_symlink(entry.path())) {
                DeleteFileA(stc::string::replace(entry.path(), '\\', '/').string().c_str());
            }
        }
    }
    catch (const std::exception& e) {
        stc::cerr("Error: "s + e.what());
    }
}


void winrootbackend::symlink_creating (QByteArray& data) {
    QDataStream dataView(&data, QIODevice::ReadOnly);
    QString QtargetCollection, QCONFIG_GAME_PATH, QCONFIG_GAME;
    std::string targetCollection, CONFIG_GAME_PATH, CONFIG_GAME;
    dataView >> QtargetCollection;
    dataView >> QCONFIG_GAME_PATH;
    dataView >> QCONFIG_GAME;
    targetCollection = QtargetCollection.toStdString();
    CONFIG_GAME_PATH = QCONFIG_GAME_PATH.toStdString();
    CONFIG_GAME = QCONFIG_GAME.toStdString();

    std::string collect = COLLECTIONS + CONFIG_GAME + "/" + targetCollection;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(collect)) {
            fs::path relative = fs::relative(entry.path(), collect);
            fs::path target_path = CONFIG_GAME_PATH / relative;
            fs::path current_dir = QCoreApplication::applicationDirPath().toStdString();
            fs::path global_target_path = current_dir / entry;
            if (fs::is_directory(entry)){
                fs::create_directories(target_path);
            }
            if (fs::exists(target_path)) {
                if (!fs::is_directory(target_path)) {
                    fs::remove(target_path);
                    stc::fs::symlink(global_target_path, target_path);
                }
            }
            else stc::fs::symlink(global_target_path, target_path);
        }
    }
    catch (const std::exception& e) {
        stc::cerr("Error: "s + e.what());
    }
}


void winrootbackend::kill () {
    if (client)
        client->close();
    QCoreApplication::exit(1);
}

