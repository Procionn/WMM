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
#include "../ModManager.h"

#include "../methods.h"
#include "../core.h"
#include "../CONSTANTS.h"
#include <hpp-archive.h>
#include <filesystem>
#include <regex>
#include <archive_entry.h>

// Save file struct:
//
// name     std::string
// version  std::string
// id       uint64_t

ModManager::ModManager() {
    update();
}


ModManager& ModManager::get() {
    static ModManager singleton;
    return singleton;
}


void ModManager::update () {
    saveFile = path();
    delete dataSaveFile;
    import_saved_data();
}


void ModManager::flush () {
    dataSaveFile->flush();
}


std::string ModManager::path () {
    return std::string(stc::cwmm::ram_mods("ModManager.wmml"));
}


void ModManager::mod_log (const std::string& path, const uint64_t id, const std::string& version) {
    std::filesystem::path logPath = get_log_path(id, version);
    ArchiveReader archive(path);
    std::filesystem::create_directories(logPath.parent_path());
    std::ofstream log(logPath);
    for (const auto& entry : archive) {
        if (archive_entry_filetype(entry) == AE_IFDIR)
            continue;
        log << archive.get_target_filename() << "\n";
    }
    log.close();
}


std::tuple<std::string, uint64_t, std::string> ModManager::regex (const std::string& filename) {
    std::string nameArchive = stc::string::get_name(filename);
    std::smatch matches;
    std::regex archiveRegex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?$)");
    if (!std::regex_match(nameArchive, matches, archiveRegex))
        throw ("ERROR:  REGULAR NAME ERROR");
    return {matches[1], std::stoi(matches[2]), matches[3]};
}


void ModManager::load (const std::string& path) {
    auto dataBlock = regex(path);
    uint64_t modId 	= std::get<1>(dataBlock);
    std::string modVersion  = std::get<2>(dataBlock);
    std::string modName 	= std::get<0>(dataBlock);

    try {
        add(modId, modVersion, modName, path);
    }
    catch (const int err) {
        switch (err) {
        case  1:
        case -1: return;
        case  0: break;
        }
    }
    mod_log(path, modId, modVersion);

    std::filesystem::path archivePath = get_path(modId, modVersion);
    std::filesystem::create_directories(archivePath.parent_path());
    if (copy)
        std::filesystem::copy(path, archivePath);
    else
        std::filesystem::rename(path, archivePath);
    dataSaveFile->flush();
}


std::string ModManager::get_path(const uint64_t id) {
    return (MODS + Core::CONFIG_GAME + "/" + std::to_string(id));
}


std::string ModManager::get_path(const uint64_t id, const std::string& version) {
    return (MODS + Core::CONFIG_GAME + "/" + std::to_string(id) + "/" + version + archiveExpansion);
}


std::string ModManager::get_log_path(const uint64_t id, const std::string& version) {
    return (ARCHIVE + Core::CONFIG_GAME + "/" + std::to_string(id) + "/" + version + EXPANSION2);
}


std::string ModManager::get_path(const std::string& name){
    return get_path(mod_data_converter(name));
}


std::string ModManager::get_path(const std::string& name, const std::string& version) {
    return get_path(mod_data_converter(name), version);
}


std::string ModManager::get_log_path(const std::string& name, const std::string& version) {
    return get_log_path(mod_data_converter(name), version);
}


bool ModManager::exists (const uint64_t id, const std::string& version) {
    Mod* ptr = bsearch(id);
    if (ptr && bsearch(ptr, version))
        return true;
    return false;
}


bool ModManager::exists (const std::string& name, const std::string& version) {
    return exists(mod_data_converter(name), version);
}


void ModManager::remove (const uint64_t id, const std::string& version) {
    ML_remove(id, version);
    std::filesystem::remove(get_path(id, version));
}


void ModManager::remove (const std::string& name, const std::string& version) {
    remove(mod_data_converter(name), version);
}


uint64_t ModManager::mod_data_converter (const std::string& modName) {
    return dictionary[modName];
}


std::string ModManager::mod_data_converter (const uint64_t modId) {
    return reverceDictionary[modId];
}


std::string ModManager::mod_recommended_version (const uint64_t modId) {
    auto* ptr = bsearch(modId);
    if (!ptr)
        throw std::runtime_error("unknown mod id");
    return ptr->recommended_version();
}



std::string ModManager::mod_recommended_version(const std::string& modName) {
    return mod_recommended_version(mod_data_converter(modName));
}
