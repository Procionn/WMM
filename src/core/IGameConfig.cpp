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
#ifdef _WIN32
    #include <windows.h>
    #include <sys/stat.h>
#endif
#include "../core.h"
#include "../methods.h"
#include "../CONSTANTS.h"
namespace fs = std::filesystem;

void WinGameConfig::dir_comparison (const std::filesystem::path& file) {

}


void WinGameConfig::symlink_deliting () {

}


void WinGameConfig::symlink_creating (const std::string& targetCollection) {

}





void NixGameConfig::dir_comparison (const std::filesystem::path& file) {
    wmml targetFile(file);
    std::vector<wmml::variant> v(GRID_WIDTH);
    while(targetFile.read(v))
        if (std::get<std::string>(v[1]) == "this")
            break;
    try {
        // pathToBackupPath   =  game/Cyberpunk 2077/
        // mixedGameDirectory =  C://Game/Cyberpunk 2077/[MGD]/
        // CONFIG_GAME_PATH   =  C://Game/Cyberpunk 2077/

        fs::path pathToBackupPath = GAME / fs::path(Core::get().core_dir_name);
        fs::path mixedGameDirectory;
        for (const auto& directory : Core::get().MGD) {
            mixedGameDirectory = Core::get().CONFIG_GAME_PATH / fs::path(directory);
            fs::path relative, pathToBackupFile, collectionFile;
            for (const auto& entry : fs::recursive_directory_iterator(mixedGameDirectory)) {
                relative = fs::relative(entry.path(), Core::get().CONFIG_GAME_PATH);
                pathToBackupFile = pathToBackupPath / relative;
                collectionFile = (COLLECTIONS + Core::CONFIG_GAME + "/" +
                                  std::get<std::string>(v[0])) / relative;
                if (!fs::exists(pathToBackupFile)) {
#ifdef _WIN32
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
#elif defined(__linux__)
                    if (fs::is_directory(entry.path()) || fs::is_symlink(entry.path()))
                        continue;
                    fs::create_directories(collectionFile.parent_path());
                    fs::rename(entry.path(), collectionFile);
#endif
                }
            }
        }
    }
    catch (const std::exception& e) {
        stc::cerr(std::string("Error: ") + e.what());
    }
}


void NixGameConfig::symlink_deliting () {
    fs::path testFile = (Core::get().CONFIG_GAME_PATH + "/" + CONST_FILE + EXPANSION);
    if (fs::exists(testFile))
        dir_comparison(testFile);
    try {
#ifdef _WIN32
        auto is_symlink = [](const std::filesystem::path& p) -> bool {
            DWORD attrs = GetFileAttributesA(p.string().c_str());
            return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
        };

        for (const auto& entry : fs::recursive_directory_iterator(Core::get().CONFIG_GAME_PATH)) {
            if (is_symlink(entry.path())) {
                DeleteFileA(stc::string::replace(entry.path(), '\\', '/').string().c_str());
            }
        }
#elif defined(__linux__)
        for (const auto& entry : fs::recursive_directory_iterator(Core::get().CONFIG_GAME_PATH)) {
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
    std::string collect = COLLECTIONS + Core::CONFIG_GAME + "/" + targetCollection;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(collect)) {
            fs::path relative = fs::relative(entry.path(), collect);
            fs::path target_path = Core::get().CONFIG_GAME_PATH / relative;
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
        stc::cerr(std::string("Error: ") + e.what());
    }
}
