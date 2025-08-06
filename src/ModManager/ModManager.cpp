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

// Save file struct:
//
// name     std::string
// version  std::string
// id       unsigned long

ModManager::ModManager() {
    saveFile = path();
    import_saved_data();
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


std::string ModManager::path () {
    return std::string(stc::cwmm::ram_mods("ModManager.wmml"));
}


void ModManager::mod_log (const std::string& path, const unsigned long id, const std::string& version) {
    std::filesystem::path logPath = get_log_path(id, version);
    ArchiveReader archive(path);
    std::filesystem::create_directories(logPath.parent_path());
    std::ofstream log(logPath);
    for (const auto& entry : archive)
        log << archive.get_target_filename();
    log.close();
}


void* ModManager::regex (const std::string& filename) {
    std::string nameArchive = stc::string::get_name(filename);
    std::smatch* matches = new std::smatch;
    std::regex archiveRegex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?\$)");
    if (!std::regex_match(nameArchive, *matches, archiveRegex))
        throw ("ERROR:  REGULAR NAME ERROR");
    // return Mod(matches[3], matches[1], std::stol(matches[2]), 0);
    return static_cast<void*>(matches);
}


void ModManager::load (const std::string& path) {
    std::smatch* dataBlock = static_cast<std::smatch*>(regex(path));
    const unsigned long modId = std::stol((*dataBlock)[2]);
    const std::string modVersion = (*dataBlock)[3];
    const std::string modName = (*dataBlock)[1];
    delete dataBlock;

    stc::cerr("ModManager::load: regex complete");

    add(modId, modVersion, modName);

    stc::cerr("ModManager::load: database adding complete");

    mod_log(path, modId, modVersion);

    stc::cerr("ModManager::load: log created");

    std::filesystem::path archivePath = get_path(modId, modVersion);
    std::filesystem::create_directories(archivePath.parent_path());
    if (copy)
        std::filesystem::copy(path, archivePath);
    else
        std::filesystem::rename(path, archivePath);

    stc::cerr("ModManager::load: data move complete");
}


std::string ModManager::get_path(const unsigned long id) {
    return (MODS + Core::CONFIG_GAME + "/" + std::to_string(id));
}


std::string ModManager::get_path(const unsigned long id, const std::string& version) {
    return (MODS + Core::CONFIG_GAME + "/" + std::to_string(id) + "/" + version + archiveExpansion);
}


std::string ModManager::get_log_path(const unsigned long id, const std::string& version) {
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


bool ModManager::exists (const unsigned long id, const std::string& version) {
    Mod* ptr = bsearch(id);
    if (ptr && bsearch(ptr, version))
        return true;
    return false;
}


bool ModManager::exists (const std::string& name, const std::string& version) {
    return exists(mod_data_converter(name), version);
}


void ModManager::remove (const unsigned long id, const std::string& version) {
    ML_remove(id, version);
    std::filesystem::remove(get_path(id, version));
}


void ModManager::remove (const std::string& name, const std::string& version) {
    remove(mod_data_converter(name), version);
}


unsigned long ModManager::mod_data_converter (const std::string& modName) {
    return dictionary[modName];
}


std::string ModManager::mod_data_converter (const unsigned long modId) {
    return reverceDictionary[modId];
}


std::string ModManager::mod_recommended_version (const unsigned long modId) {
    auto* ptr = bsearch(modId);
    if (!ptr)
        throw std::runtime_error("unknown mod id");
    return ptr->recommended_version();
}



std::string ModManager::mod_recommended_version(const std::string& modName) {
    return mod_recommended_version(mod_data_converter(modName));
}
