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
}


void winrootbackend::connection () {
    if (client) {
        server.nextPendingConnection()->close();
        return;
    }
    client = server.nextPendingConnection();

    connect(client, &QLocalSocket::disconnected, []{
        QCoreApplication::exit(0);
    });

    connect(client, &QLocalSocket::readyRead, [this]{
        QDataStream pack(client);
        IpcHeader ipc;
        if (client->bytesAvailable() < sizeof(IpcHeader)) {
            stc::cerr("data lose"s);
            return;
        }

        pack.readRawData(reinterpret_cast<char*>(&ipc), sizeof(ipc));
        if (ipc.hash != IpcHeader::hash || ipc.version != IpcHeader::version) {
            stc::cerr(std::to_string(ipc.hash) +" "s + std::to_string(IpcHeader::hash) + "\n"s
                      + std::to_string(ipc.version) + " "s + std::to_string(IpcHeader::version));
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
                                     const std::string& CONFIG_GAME_PATH,
                                     const std::string& CONFIG_GAME, const QStringList& MGD) {
    const std::string collectionName = [] (const fs::path& file) {
        wmml targetFile(file);
        std::vector<wmml::variant> v(GRID_WIDTH);
        while (targetFile.read(v))
            if (std::get<std::string>(v[1]) == "this")
                break;
        return std::get<std::string>(v[0]);
    }(file);
    const fs::path gamePath = CONFIG_GAME_PATH;
    const fs::path backupDir = GAME / fs::path(core_dir_name);
    const fs::path collectionDir = COLLECTIONS + CONFIG_GAME + "/" + collectionName;
    fs::path mgd, relative, inBackupFile, inCollectionFile;
    // backupDir    =  game/Cyberpunk 2077/
    // mgd          =  C://Game/Cyberpunk 2077/[MGD]/
    // gamePath     =  C://Game/Cyberpunk 2077/
    auto is_symlink = [] (const std::filesystem::path& p) -> bool {
        DWORD attrs = GetFileAttributesA(p.string().c_str());
        return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
    };
    auto is_directory = [] (const std::filesystem::path& p) -> bool {
        struct stat info;
        if (stat(p.string().c_str(), &info) == 0 && S_ISDIR(info.st_mode))
            return true;
    };

    try {
        for (const auto& branch : MGD) {
            mgd = gamePath / branch.toStdString();
            for (const auto& entry : fs::recursive_directory_iterator(mgd)) {
                relative = fs::relative(entry.path(), gamePath);
                inBackupFile = backupDir / relative;
                if (!fs::exists(inBackupFile)) {
                    if (!is_directory(inBackupFile) && !is_symlink(inBackupFile)) {
                        inCollectionFile = collectionDir / relative;
                        fs::create_directories(inCollectionFile.parent_path());
                        if (fs::exists(inCollectionFile))
                            fs::remove(inCollectionFile);
#ifndef NDEBUG
                        stc::cerr(entry.path().string() + std::string(" file moved to ") +
                                  inCollectionFile.string());
#endif
                        fs::copy_file(entry.path(), inCollectionFile);
                        fs::remove(entry.path());
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        stc::cerr(std::string("Error: ") + e.what());
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
                DeleteFileA(entry.path().string().c_str());
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
        fs::path entry;
        for (auto i = fs::recursive_directory_iterator(collect);
             i != fs::recursive_directory_iterator(); ++i) {
            entry = (*i).path();
            fs::path relative = fs::relative(entry, collect);
            fs::path target_path = QCONFIG_GAME_PATH.toStdString() / relative;
            fs::path current_dir = QCoreApplication::applicationDirPath().toStdString();
            fs::path global_target_path = current_dir / entry;
            if (fs::is_directory(entry)){
                if (!fs::exists(target_path)) {
                    stc::fs::symlink(global_target_path, target_path);
                    i.disable_recursion_pending();
                }
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

