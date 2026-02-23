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
#include "methods.h"
#include "CONSTANTS.h"
#include "core.h"

#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <QDesktopServices>
#include <QUrl>
#include <wmml.h>
#ifdef _WIN32
    #include <windows.h>
#endif

void stc::string::replace (std::string& input, const char replaceable, const char target) {
    for (int i = 0; input[i]; ++i)
        if (input[i] == replaceable)
            input[i] = target;
}

std::filesystem::path stc::string::replace (const std::filesystem::path& input, const char& replaceable, const char& target) {
    std::string sorce = input.string();
    std::replace(sorce.begin(), sorce.end(), replaceable, target);
    std::filesystem::path out = sorce;
    return out;
}

std::string stc::string::get_name(const std::string& path) {
    std::size_t part = path.find_last_of('/');
    std::size_t expansion = path.find_last_of('.');
    ++part;
    return path.substr(part, (expansion - part));
}

std::string stc::string::get_name(const std::string& path, std::string& directory, std::string& exp) {
    std::size_t part = path.find_last_of('/');
    std::size_t expansion = path.find_last_of('.');
    ++part;
    directory = path.substr(0, part);
    exp = path.substr(expansion);
    return path.substr(part, (expansion - part));
}



std::string stc::cwmm::backup_path () {
    return (GAME + "/" + CConfigs::CONFIG_GAME);
}

std::string stc::cwmm::ram_preset () {
    return (RAM + CConfigs::CONFIG_GAME + "/" + PRESETS);
}

std::string stc::cwmm::ram_preset (const std::string& name) {
    return (RAM + CConfigs::CONFIG_GAME + "/" + PRESETS + name + EXPANSION);
}

std::string stc::cwmm::ram_collection () {
    return (RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS);
}

std::string stc::cwmm::ram_collection (const std::string& name) {
    return (RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS + name + EXPANSION);
}

std::string stc::cwmm::ram_mods() {
    return (MODS + CConfigs::CONFIG_GAME);
}

std::string stc::cwmm::ram_mods(const std::string& name) {
    return (MODS + CConfigs::CONFIG_GAME + "/" + name);
}

std::string stc::cwmm::cortege_path(const std::string& name, const uint64_t& id) {
    return (MODS + Core::CONFIG_GAME + "/" + std::to_string(id) + "/" + name + CORTEGE);
}



std::string stc::cwmm::modsURL (const std::string& id) {
    std::string tmp = Core::CONFIG_URL;
    size_t mark1 = tmp.find_last_of('[');
    size_t mark2 = tmp.find_last_of(']');
#ifndef NDEBUG
    std::cout << "Opening URL: " << (tmp.substr(0, mark1) + id +  tmp.substr(mark2 + 1)) << std::endl;
#endif
    return (tmp.substr(0, mark1) + id +  tmp.substr(mark2 + 1));
}



void stc::fs::symlink (const std::filesystem::path& file, const std::filesystem::path& name) {
    std::filesystem::path newName = stc::string::replace(name, '\\', '/');
    std::filesystem::path newFile = stc::string::replace(file, '\\', '/');
    std::filesystem::create_directories(newName.parent_path());
    try {
#ifdef _WIN32
        if (!CreateSymbolicLinkW(newName.wstring().c_str(), newFile.wstring().c_str(), 0x0)) {
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
            stc::cerr("Error with linking [" + newName.string() + "] to [" + newFile.string() + "]");
            stc::cerr(messageBuffer);
            LocalFree(messageBuffer);
        }
#elif linux
        std::filesystem::create_symlink(newFile, newName);
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void stc::fs::remove_all (const std::filesystem::path& path) {
    if (std::filesystem::exists(path)) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_directory(entry.path()))
                continue;
            else {
                std::filesystem::permissions(entry, std::filesystem::perms::owner_write,
                                             std::filesystem::perm_options::add);
                std::filesystem::remove(entry.path());
            }
        }
        std::filesystem::remove_all(path);
    }
}



void stc::net::openURL (const std::string& url) {
    QDesktopServices::openUrl(QUrl(url.c_str()));
}



void stc::wmm::new_object (const std::filesystem::path& path) {
    std::filesystem::create_directories(path.parent_path());
    wmml file(path, GRID_WIDTH);
}
