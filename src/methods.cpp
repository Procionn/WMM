#include "methods.h"
#include "CONSTANTS.h"
#include "core.h"

#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstdlib>
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

std::string stc::cwmm::ram_mods_info(const std::string& name) {
    return (MODS + CConfigs::CONFIG_GAME + "/" + name + "/" + WML + name + EXPANSION2);
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
        if (!CreateSymbolicLinkW(newName.wstring().c_str(), newFile.wstring().c_str(), 0x2))
            std::cerr << "AHTUNG!!! I'm facking hate windows!!!"
                      << "\n" << newName << "\n" << newFile << std::endl;
#elif defined(__linux__)
        std::filesystem::create_symlink(newFile, newName);
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void stc::fs::remove_all (const std::filesystem::path& path) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (std::filesystem::is_directory(entry.path())) continue;
        else {
            std::filesystem::permissions(entry,
                                         std::filesystem::perms::owner_write,
                                         std::filesystem::perm_options::add);
            std::filesystem::remove(entry.path());
        }
    }
    std::filesystem::remove_all(path);
}



void stc::net::openURL (const std::string& url) {
#ifdef _WIN32
    std::string command = "start " + url;
#elif __linux__
    std::string command = "xdg-open " + url;
#else
    std::cerr << "System not found!" << std::endl;
    return;
#endif
    std::system(command.c_str());
}



void stc::wmm::new_object (const std::filesystem::path& path) {
    std::filesystem::create_directories(path.parent_path());
    wmml file(path, GRID_WIDTH);
}
