/*
 *  Copyright (C) 2025 Procion ByProcion@gmail.com              *
 *  This file is part of Wirus Mod Manager.
 *
 *  This program is free software: you can redistribute it and>
 *  it under the terms of the GNU General Public License as pu>
 *  the Free Software Foundation, either version 2 of the Lice>
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be us>
 *  but WITHOUT ANY WARRANTY; without even the implied warrant>
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 *
 */

#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <QDateTime>
#include <mutex>
#include <windows.h>
#ifndef NDEBUG
#include <iostream>
#endif

using namespace std::literals;
namespace stc{

inline std::filesystem::path replace (const std::filesystem::path& input, const char& replaceable, const char& target) {
    std::string sorce = input.string();
    std::replace(sorce.begin(), sorce.end(), replaceable, target);
    std::filesystem::path out = sorce;
    return out;
}
inline std::stting replace (std::string source, const char& replaceable, const char& target) {
    std::replace(source.begin(), source.end(), replaceable, target);
    return source;
}


namespace {
    std::ofstream& log () {
        if (!std::filesystem::exists("logs/"))
            std::filesystem::create_directories("logs/");
        static std::ofstream logFile(("logs/"s + "root"s + QDateTime::currentDateTime().toString("dd_MM_yy-hh_mm_ss").toStdString() + ".log").c_str());
        return logFile;
    }
    std::mutex thisIsMutex;
}

template<typename T>
void cerr(const T& t) noexcept {
#ifndef NDEBUG
     std::cerr << t << std::endl;
#endif
    std::lock_guard<std::mutex> lock(thisIsMutex);
    log() << t << std::endl << std::flush;
}

inline void symlink (const std::filesystem::path& file, const std::filesystem::path& name) {
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
            cerr("Error with linking [" + newName.string() + "] to [" + newFile.string() + "]");
            cerr(messageBuffer);
            LocalFree(messageBuffer);
        }
#elif linux
        std::filesystem::create_symlink(newFile, newName);
#endif
    }
    catch (const std::exception& e) {
        cerr("Error: "s + e.what() + "\n"s);
    }
}

}
