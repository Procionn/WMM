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
#ifndef METHODS_H
#define METHODS_H

#include <string>
#include <filesystem>
#ifndef NDEBUG
#include <iostream>
#endif

namespace stc {
    namespace string {
        void replace (std::string& input, const char replaceable, const char target);
        std::filesystem::path replace (const std::filesystem::path& input, const char& replaceable, const char& target);
        std::string get_name(const std::string& path);
        std::string get_name(const std::string& path, std::string& directory, std::string& expansion);
    }
    namespace cwmm { // Constants WMM
        std::string backup_path ();
        std::string ram_preset ();
        std::string ram_preset (const std::string& name);
        std::string ram_collection ();
        std::string ram_collection (const std::string& name);
        std::string modsURL(const std::string& id);
        std::string ram_mods();
        std::string ram_mods(const std::string& name);
        std::string ram_mods_info(const std::string& name);

    }
    namespace fs {   // file system
        void symlink(const std::filesystem::path& file, const std::filesystem::path& name);
        void remove_all(const std::filesystem::path& path);
    }
    namespace net {
        void openURL (const std::string& url);
    }
    namespace wmm {
        void new_object(const std::filesystem::path& path);
    }

    template<typename T>
    void cerr(const T& t) {
#ifndef NDEBUG
        std::cerr << t << std::endl;
#endif
    }
}

#endif // METHODS_H
