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

#include "../CONSTANTS.h"
#include "../core.h"
#include "../dialog_window/CCortegeWindow.h"
#include "../methods.h"
#include <QLibrary>
#include <archive_entry.h>
#include <filesystem>
#include <hpp-archive.h>
#include <regex>
#include <wmml.h>

// Save file struct:
//
// name     std::string
// version  std::string
// id       uint64_t
// type     char        0 - mod, 1 - cortege (crt)

typedef void (*func)(ModManager*);
ModManager::ModManager() {
    Core::get().set_default("WMM_MOD_MANAGER_TYPE", "true");
    std::string type = Core::configs["WMM_MOD_MANAGER_TYPE"];
    if (type == "true")
        copy = true;
    else
        copy = false;
    update();

    func f = (func)(QLibrary::resolve(LIB, "start_modmanager_api"));
    if (f)
        f(this);
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


bool ModManager::get_copy () {
    return copy;
}


void ModManager::set_copy (const bool& status) {
    if (status == copy)
        return;
    copy = status;
    Core::configs["WMM_MOD_MANAGER_TYPE"] = std::string(status ? "true" : "false");
    Core::get().overwriting_config_data();
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
    uint64_t modId = std::get<1>(dataBlock);
    std::string modVersion = std::get<2>(dataBlock);
    std::string modName = std::get<0>(dataBlock);

    std::string crtName, baseVersion;
    std::string inDatabaseValue = reverceDictionary[modId];
    if (!inDatabaseValue.empty() && modName != inDatabaseValue) {
        std::tuple<std::string, std::string, std::string> ret;
        if (exists(modId, modVersion)) // start creating cortege window
            ret = CCortegeWindow::create(modVersion, modName, modId);
        else // start cortege question window
            ret = CCortegeWindow::question(modVersion, modName, modId);

        std::string version = std::get<0>(ret);
        if (version.empty())
            return;
        modVersion = std::move(version);
        crtName = std::get<1>(ret);
        baseVersion = std::get<2>(ret);
    }
    add(modId, modVersion, modName);

    mod_log(path, modId, modVersion);
    if (!crtName.empty())
        create_cortege({baseVersion, modVersion}, crtName, modId);

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


std::string ModManager::get_cortege_path(const uint64_t id, const std::string& name) {
    return stc::cwmm::cortege_path(name, id);
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


bool ModManager::is_cortege(const uint64_t id,  const std::string& version) {
    Mod* ptr = bsearch(id);
    if (ptr) {
        ModInfo* obj = bsearch(ptr, version);
        if (obj && !obj->isModInfo)
            return true;
    }
    return false;
}


std::vector<ModInfo*> ModManager::get_cortege_list(const uint64_t id,  const std::string& version) {
    if (is_cortege(id, version)) {
        Mod* ptr = bsearch(id);
        ModCortege* obj = static_cast<ModCortege*>(bsearch(ptr, version));
        std::vector<ModInfo*> list;
        list.reserve(obj->dependence.size());
        for (const auto& entry : obj->dependence)
            list.emplace_back(bsearch(ptr, entry));
        return list;
    }
    else throw std::runtime_error("Object is not cortege");
}


void ModManager::remove (const uint64_t id, const std::string& version) {
    if (is_cortege(id, version))
        std::filesystem::remove(get_cortege_path(id, version));
    else
        std::filesystem::remove(get_path(id, version));
    ML_remove(id, version);
}


void ModManager::remove (const std::string& name, const std::string& version) {
    remove(mod_data_converter(name), version);
}


void ModManager::remove (const uint64_t id) {
    try {
        stc::fs::remove_all(get_path(id));
        ML_remove(id);
    }
    catch (const std::exception& err) {
        stc::cerr(err.what());
    }
}


void ModManager::remove (const std::string& name) {
    remove(mod_data_converter(name));
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
