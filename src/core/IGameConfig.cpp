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
#include "IGameConfig.h"
#include <wmml.h>
#include <vector>
#include <QCoreApplication>
#include <QRandomGenerator>
#include <QLocalSocket>
#include <mutex>
#ifdef WIN64
    #include <windows.h>
    #include <sys/stat.h>
#endif
#include "../core.h"
#include "../methods.h"
#include "../CONSTANTS.h"
#include "InterprocessData.h"

namespace fs = std::filesystem;

namespace {
    bool sockeEstablished = false;
    std::mutex m;

    void platform_test () {
#ifdef WIN64
        stc::cerr("sending a command to admin process");
#else
        std::runtime_error("This programm--keyboard=sdk module is not supported on this platform!");
#endif
    }


    QString get_token () {
        static QByteArray token;
        if (token.isEmpty()) {
            QByteArray bytes(16, Qt::Uninitialized);
            QRandomGenerator::system()->generate(
                bytes.begin(), bytes.end());
            token = bytes.toHex();
        }
        return token;
    }


    QString get_pipe () {
        static QString pipe;
        if (pipe.isEmpty()) {
            QByteArray bytes(16, Qt::Uninitialized);
            QRandomGenerator::system()->generate(
                bytes.begin(), bytes.end());
            pipe = bytes.toHex();
        }
        return pipe;
    }


    QLocalSocket* get_admin_process () {
        static QLocalSocket process;
#ifdef WIN64
        if (process.state() != QLocalSocket::ConnectedState) {
            STARTUPINFOW si{sizeof(si)};
            PROCESS_INFORMATION pi;
            std::wstring cmd = std::wstring(L"\"WinRoot.exe\" --pipe=") + get_pipe().toStdWString() + L" --token=" + get_token().toStdWString();
            if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr,
                                FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {

                LPSTR messageBuffer = nullptr;
                FormatMessageA(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM,
                    nullptr,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPSTR)&messageBuffer,
                    0,
                    nullptr
                    );
                stc::cerr("permission to run the root process was not obtained: ");
                stc::cerr(messageBuffer);
                LocalFree(messageBuffer);
                std::runtime_error("Error starting root process");
            }

            process.connectToServer(get_pipe());
            if (!process.waitForConnected(30000))
                throw std::runtime_error("Couldn't start admin process");
        }

#else
        throw std::runtime_error("This programm module is not supported on this platform!");
#endif
        sockeEstablished = true;
        return &process;
    }


    void send_command (QByteArray& data, IpcHeader* ipc) {
        std::lock_guard<std::mutex> guard(m);
        auto rootProcess = get_admin_process();

        QByteArray metaInfo;
        QDataStream out(&metaInfo, QIODevice::WriteOnly);
        out << get_token();
        out << data;

        ipc->dataSize = metaInfo.size();

        QByteArray pack;
        pack.append(reinterpret_cast<char*>(ipc), sizeof(IpcHeader));
        pack.append(metaInfo);

        rootProcess->write(pack);
        rootProcess->flush();
    }
}


WinGameConfig::~WinGameConfig () {
    if (sockeEstablished) {
        platform_test();
        IpcHeader ipc;
        ipc.comand = ComandList::kill;
        QByteArray data;
        send_command(data, &ipc);
        sockeEstablished = false;
    }
}


void WinGameConfig::dir_comparison (const std::filesystem::path& file) {
    platform_test();
    IpcHeader ipc;
    ipc.comand = ComandList::dir_cmp;
    QByteArray data;
    QDataStream dataView(&data, QIODevice::WriteOnly);
    dataView << QString::fromStdString(file.string());
    dataView << QString::fromStdString(Core::get().get_game_config("core_dir_name"));
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME_PATH"));
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME"));
    QStringList list;
    for (auto entry : Core::get().get_MGD())
        list.append(QString::fromStdString(entry));
    dataView << list;
    send_command(data, &ipc);
}


void WinGameConfig::symlink_deliting () {
    platform_test();
    IpcHeader ipc;
    ipc.comand = ComandList::symlnk_del;
    QByteArray data;
    QDataStream dataView(&data, QIODevice::WriteOnly);
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME_PATH"));
    dataView << QString::fromStdString(Core::get().get_game_config("core_dir_name"));
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME"));
    QStringList list;
    for (auto entry : Core::get().get_MGD())
        list.append(QString::fromStdString(entry));
    dataView << list;
    send_command(data, &ipc);
}


void WinGameConfig::symlink_creating (const std::string& targetCollection) {
    platform_test();
    Core::get().restorer();
    IpcHeader ipc;
    ipc.comand = ComandList::symlnk_create;
    QByteArray data;
    QDataStream dataView(&data, QIODevice::WriteOnly);
    dataView << QString::fromStdString(targetCollection);
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME_PATH"));
    dataView << QString::fromStdString(Core::get().get_game_config("CONFIG_GAME"));
    send_command(data, &ipc);
}





void NixGameConfig::dir_comparison (const std::filesystem::path& file) {
    const std::string collectionName = [] (const fs::path& file) {
        wmml targetFile(file);
        std::vector<wmml::variant> v(GRID_WIDTH);
        while (targetFile.read(v))
            if (std::get<std::string>(v[1]) == "this")
                break;
        return std::get<std::string>(v[0]);
    }(file);
    const fs::path gamePath = Core::get().get_game_config("CONFIG_GAME_PATH");
    const fs::path backupDir = GAME / fs::path(Core::get().get_game_config("core_dir_name"));
    const fs::path collectionDir = COLLECTIONS + Core::config("WMM_CONFIG_GAME") + "/" + collectionName;
    fs::path mgd, relative, inBackupFile, inCollectionFile;
    // backupDir    =  game/Cyberpunk 2077/
    // mgd          =  C://Game/Cyberpunk 2077/[MGD]/
    // gamePath     =  C://Game/Cyberpunk 2077/

    try {
        for (const auto& branch : Core::get().get_MGD()) {
            mgd = gamePath / branch;
            for (const auto& entry : fs::recursive_directory_iterator(mgd)) {
                relative = fs::relative(entry.path(), gamePath);
                inBackupFile = backupDir / relative;
                if (!fs::exists(inBackupFile)) {
                    if (!fs::is_directory(inBackupFile) && !fs::is_symlink(inBackupFile)) {
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



void NixGameConfig::symlink_deliting () {
    fs::path testFile = (Core::get().get_game_config("CONFIG_GAME_PATH") + "/" + CONST_FILE + EXPANSION);
    if (fs::exists(testFile))
        dir_comparison(testFile);
    try {
#ifdef _WIN32
        auto is_symlink = [](const std::filesystem::path& p) -> bool {
            DWORD attrs = GetFileAttributesA(p.string().c_str());
            return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
        };

        for (const auto& entry :
             fs::recursive_directory_iterator(Core::get().get_game_config("CONFIG_GAME_PATH")))
        {
            if (is_symlink(entry.path())) {
                DeleteFileA(entry.path().string().c_str());
            }
        }
#elif defined(__linux__)
        for (const auto& entry : fs::recursive_directory_iterator(
                 Core::get().get_game_config("CONFIG_GAME_PATH"))) {
            const auto& status = entry.symlink_status();
            if (fs::is_symlink(status)) {
#ifndef NDEBUG
                std::cout << "is symlink -> " << entry.path() << std::endl;
#endif
                fs::remove(entry);
            }
        }
#endif
    }
    catch (const std::exception& e) {
        stc::cerr(std::string("Error: ") + e.what());
    }
}


void NixGameConfig::symlink_creating (const std::string& targetCollection) {
    Core::get().restorer();
    fs::path collect = COLLECTIONS + Core::config("WMM_CONFIG_GAME") + "/" + targetCollection;
    try {
        fs::path entry;
        for (auto i = fs::recursive_directory_iterator(collect);
                  i != fs::recursive_directory_iterator(); ++i) {
            entry = (*i).path();
            fs::path relative = fs::relative(entry, collect);
            fs::path target_path = Core::get().get_game_config("CONFIG_GAME_PATH") / relative;
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
        stc::cerr(std::string("Error: ") + e.what());
    }
}
