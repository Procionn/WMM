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
#include "../core.h"

// #ifndef NDEBUG
    #include <iostream>
    #include <iomanip>
// #endif
#ifdef _WIN32
    #include <windows.h>
    #include <sys/stat.h>
#endif
#include <QCoreApplication>

#include "../CONSTANTS.h"
#include "../methods.h"

namespace fs = std::filesystem;


CGameConfig::CGameConfig () {
    update_data_from_file();
}


void CGameConfig::update_data_from_file () {
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        std::vector<wmml::variant> v(wmml_size);
        while (file.read(v)) {
            if (std::get<std::string>(v[0]) == Core::CONFIG_GAME)
                break;
        }
        CONFIG_GAME_PATH = std::get<std::string>(v[2]);
        CONFIG_EXECUTABLE_FILE = std::get<std::string>(v[1]);
        core_dir_name = stc::string::get_name(CONFIG_GAME_PATH);
    }
    else {
        CONFIG_GAME_PATH = "";
        CONFIG_EXECUTABLE_FILE = "";
    }

    std::ifstream readedFile ((GAMES + Core::CONFIG_GAME + EXPANSION3).c_str());
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
             if (parameter == config_string[0]) GAME_CORE_DIR_STAGE = std::stoi(indicator);
        else if (parameter == config_string[1]) OMD.push_back(indicator);
        else if (parameter == config_string[2]) MGD.push_back(indicator);
        else if (parameter == config_string[3]) CONFIG_URL =  indicator;
    }
}


void CGameConfig::write(wmml& input, std::string str) {
    std::vector<wmml::variant> v(wmml_size);
    v[0] = Core::CONFIG_GAME;
    v[1] = str;
    if (!GAME_CORE_DIR_STAGE)
        stc::cerr("error, GAME_CORE_DIR_STAGE not valid!");
    for (int counter = GAME_CORE_DIR_STAGE; counter != 0; --counter) {
        size_t part = str.find_last_of('/');
        str = str.substr(0, part);
    }
    v[2] = str;
    input.write(v);
}


void CGameConfig::save_game_path (const std::string& path) {
    // create a save of game in save.wmml file
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        std::vector<wmml::variant> v(wmml_size);
        for (int counter = 0; file.read(v); ++counter) {
            if (std::get<std::string>(v[0]) == Core::CONFIG_GAME) {
                file.remove_object(counter);
                write(file, path);
            }
        }
    }
    else {
        wmml file(SAVE, wmml_size);
        write(file, path);
    }
}


void CGameConfig::game_dir_backup () {
    fs::path target_path = GAME + core_dir_name;
    if (fs::exists(target_path))
        stc::fs::remove_all(target_path);
    try {
        for (const auto& directory : MGD) {
            fs::path sorce_path = (CONFIG_GAME_PATH + "/" + directory);
            for (const auto& entry : fs::recursive_directory_iterator(sorce_path)) {
                if (fs::is_regular_file(entry.path())) {
                    fs::path new_target_path(fs::relative(entry.path(), sorce_path));
                    new_target_path = target_path / (directory) / new_target_path;
                    fs::create_directories(new_target_path.parent_path());
                    fs::copy_file(stc::string::replace(entry.path(), '\\', '/'),
                                  stc::string::replace(new_target_path, '\\', '/'));
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}


void CGameConfig::symlink_deliting () {
    fs::path testFile = (CONFIG_GAME_PATH + "/" + CONST_FILE + EXPANSION);
#ifndef NDEBUG
    std::cout << testFile << std::endl;
#endif
    if (fs::exists(testFile)) dir_comparison(testFile);
    try {
#ifdef _WIN32
        auto is_symlink = [](const std::filesystem::path& p) -> bool {
            DWORD attrs = GetFileAttributesA(p.string().c_str());
            return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
        };

        for (const auto& entry : fs::recursive_directory_iterator(CONFIG_GAME_PATH)) {
            if (is_symlink(entry.path())) {
                DeleteFileA(stc::string::replace(entry.path(), '\\', '/').string().c_str());
            }
        }
#elif defined(__linux__)
        for (const auto& entry : fs::recursive_directory_iterator(CONFIG_GAME_PATH)) {
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
        std::cerr << "Error: " << e.what() << "\n";
    }
}


void CGameConfig::dir_comparison (const std::filesystem::path& file) {
    wmml targetFile(file);
    std::vector<wmml::variant> v(GRID_WIDTH);
    while(targetFile.read(v))
        if (std::get<std::string>(v[0]) == "this") break;
    try {
        // targetpath       =  game/Cyberpunk 2077/
        // pt               =  C://Game/Cyberpunk 2077/[MGD]/
        // CONFIG_GAME_PATH =  C://Game/Cyberpunk 2077/

        std::string targetpath = GAME + "/" + core_dir_name;
        for (const auto& directory : MGD) {
            std::string pt = CONFIG_GAME_PATH + "/" + directory;
            for (const auto& entry : fs::recursive_directory_iterator(pt)) {
                fs::path relative = fs::relative(entry.path(), CONFIG_GAME_PATH);
                fs::path target_path = targetpath / relative;
                fs::path backup_path = (COLLECTIONS + Core::CONFIG_GAME + "/" +
                                        std::get<std::string>(v[0])) / relative;
                if (!fs::exists(target_path)) {
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

                    fs::create_directories(backup_path.parent_path());
                    if (fs::exists(backup_path))
                        fs::remove(backup_path);
                    fs::rename(stc::string::replace(entry.path(), '\\', '/'),
                               stc::string::replace(backup_path, '\\', '/'));
#elif defined(__linux__)
                    if (fs::is_directory(entry.path()) || fs::is_symlink(entry.path()))
                        continue;
                    fs::rename(stc::string::replace(entry.path(), '\\', '/'),
                               stc::string::replace(backup_path, '\\', '/'));
#endif
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}


void CGameConfig::symlink_creating (const std::string& targetCollection) {
    restorer();
    std::string collect = COLLECTIONS + Core::CONFIG_GAME + "/" + targetCollection;
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
        std::cerr << "Error: " << e.what() << "\n";
    }
}


void CGameConfig::game_recovery () {
    symlink_deliting();

    auto deleteig_cycle = [=](std::vector<std::string>& vector) -> void {
        for (const auto& target : vector) {
            std::string path = CONFIG_GAME_PATH + "/" + target;
            if (!fs::exists(path))
                continue;
            stc::fs::remove_all(path);
        }
    };

    deleteig_cycle(OMD);
    deleteig_cycle(MGD);
    std::string backup = stc::cwmm::backup_path();
    for (const auto& entry : fs::recursive_directory_iterator(backup)) {
        if (fs::is_directory(entry.path()))
            continue;
        fs::path relative = fs::relative(entry.path(), backup);
        fs::path path = (CONFIG_GAME_PATH) / relative;
        fs::create_directories(path.parent_path());
        fs::copy(entry.path(), path);
    }
}


void CGameConfig::restorer () {
    for (const auto& entry : fs::recursive_directory_iterator(stc::cwmm::backup_path())) {
        fs::path relative = fs::relative(entry.path(), stc::cwmm::backup_path());
        fs::path target = CONFIG_GAME_PATH / relative;
        if (!fs::exists(target)) {
            fs::path global_backup = QCoreApplication::applicationDirPath().toStdString() / entry.path();
#ifndef NDEBUG
            std::cout << std::left << std::setw(100)
                      << global_backup << " -> " << target << std::endl;
#endif
            fs::copy(global_backup, target);
        }

    }
}
