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
#include <filesystem>
#include <wmml.h>
#include "../core.h"
#include "hpp-archive.h"
#include "../dialog_window/unificator.h"
#include "../patterns/WaitingWindow.h"

namespace {
    int modinfo_cmp (const void* a, const void* b) {
        const ModInfo* arg1 = static_cast<const ModInfo*>(a);
        const ModInfo* arg2 = static_cast<const ModInfo*>(b);
        return arg1->modVersion.compare(arg2->modVersion);
    }

    int mod_cmp (const void* a, const void* b) {
        const Mod* arg1 = static_cast<const Mod*>(a);
        const Mod* arg2 = static_cast<const Mod*>(b);
        return arg1->modId > arg2->modId ?  1 :
               arg1->modId < arg2->modId ? -1 : 0;
    }
}



ModList::~ModList () {
    delete dataSaveFile;
}


Mod* ModList::bsearch (const uint64_t& modId) {
    auto key = Mod(modId);
    return static_cast<Mod*>(std::bsearch(&key,
                                           list.data(),
                                           list.size(),
                                           sizeof(Mod),
                                           &mod_cmp));
}


ModInfo* ModList::bsearch (Mod* ptr, const std::string& modVersion) {
    auto key = ModInfo(modVersion, 0);
    return static_cast<ModInfo*>(std::bsearch(&key,
                                               ptr->versions->data(),
                                               ptr->versions->size(),
                                               sizeof(ModInfo),
                                               &modinfo_cmp));
}


void ModList::import_saved_data () {
    if (Core::CONFIG_GAME == "None")
        return;
    if (!list.empty())
        list.clear();
    if (std::filesystem::exists(saveFile)) {
        dataSaveFile = new wmml(saveFile);
        std::vector<wmml::variant> v(gridSize);
        list.reserve(dataSaveFile->height());
        while(dataSaveFile->read(v))
            add_in_ram(static_cast<void*>(&v));
        delete dataSaveFile;
        dataSaveFile = new wmml(saveFile);
    }
    else {
        std::filesystem::create_directories(std::filesystem::path(saveFile).parent_path());
        dataSaveFile = new wmml(saveFile, gridSize);
        dataSaveFile->flush();
    }
}


void ModList::add(const uint64_t& modId, const std::string&& modVersion,
                  const std::string&& modName) {
    add_in_ram(modId, modVersion, modName);
    add_in_rom(modId, modVersion, modName);
    ++localId;
}


void ModList::add(const uint64_t& modId, std::string& modVersion,
                  const std::string& modName, const std::string& path) {
    add_in_ram(modId, modVersion, modName, path);
    add_in_rom(modId, modVersion, modName);
    ++localId;
}


void ModList::add_in_ram(const uint64_t& modId, const std::string& modVersion,
                         const std::string& modName) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr)
            throw Core::lang["LANG_LABEL_MOD_EXISTS"];
        else {
            ptr->versions->emplace_back(modVersion, localId);
            std::qsort(ptr->versions->data(), ptr->versions->size(),
                       sizeof(ModInfo), &modinfo_cmp);
        }
    }
    else {
        list.emplace_back(modVersion, modId, localId);
        std::qsort(list.data(), list.size(), sizeof(Mod), &mod_cmp);
    }
    dictionary[modName] = modId;
    reverceDictionary[modId] = modName;
}


void ModList::add_in_ram(const uint64_t& modId, std::string& modVersion,
                         const std::string& modName, const std::string& path) {
    Mod* ptr = bsearch(modId);
    bool modified = false;
    if (ptr) {
        if (modName != reverceDictionary[modId]) {
            modVersion = mod_archive_unificate(path, modId, ptr, modVersion, modName);
            modified = true;
        }
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr)
            throw Core::lang["LANG_LABEL_MOD_EXISTS"];
        else {
            ptr->versions->emplace_back(modVersion, localId);
            std::qsort(ptr->versions->data(), ptr->versions->size(),
                       sizeof(ModInfo), &modinfo_cmp);
        }
    }
    else {
        list.emplace_back(modVersion, modId, localId);
        std::qsort(list.data(), list.size(), sizeof(Mod), &mod_cmp);
    }
    if (!modified) {
        dictionary[modName] = modId;
        reverceDictionary[modId] = modName;
    }
}


void ModList::add_in_rom(const uint64_t& modId, const std::string& modVersion, const std::string& modName) {
    if (dataSaveFile) {
        std::vector<wmml::variant> v{modName, modVersion, modId};
        dataSaveFile->write(v);
    }
    else
        std::runtime_error("BD file is not open");
}


void ModList::add_in_ram (const void* v) {
    const auto* c = static_cast<const std::vector<wmml::variant>*>(v);
    add_in_ram(std::get<uint64_t>(c->at(2)), std::get<std::string>(c->at(1)),
               std::get<std::string>(c->at(0)));
}


void ModList::ML_remove (const uint64_t& modId, const std::string& modVersion) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr) {
            auto index = version_ptr - ptr->versions->data(); // счёт от нуля
            dataSaveFile->remove_object(ptr->versions->data()[index].localId);
            ptr->versions->erase(ptr->versions->begin() + index);
            if (ptr->versions->empty()) {
                // в таком случае нужно уничтожить весь Mod объект
                auto second_index = ptr - list.data();
                list.erase(list.begin() + second_index);
            }
        }
        else
            throw ("Target mod version is not exists");
    }
    else
        throw ("Target mod is not exists");
}


void ModList::ML_remove (const uint64_t& modId) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        for (const ModInfo& entry : *ptr->versions)
            dataSaveFile->remove_object(entry.localId);
        auto second_index = ptr - list.data();
        list.erase(list.begin() + second_index);
    }
    else
        throw ("Target mod is not exists");
}


const std::vector<Mod>& ModList::all_mods_list() {
   return list;
}


const std::vector<std::string_view> ModList::all_versions_list (const uint64_t& modId) {
    auto* mod = bsearch(modId);
    assert(mod->versions);
    std::vector<std::string_view> versionList;
    versionList.reserve(mod->versions->size());
    // for (const auto& entry : mod->versions)
    for (const auto& entry : *mod->versions)
        versionList.emplace_back(entry.modVersion);
    return versionList;
}


std::string ModList::mod_archive_unificate (const std::string& path, const uint64_t& modId, Mod* ptr,
                                            const std::string& modVersion, const std::string& modName) {
    auto version = unificator::start(static_cast<void*>(ptr->versions), modName, modVersion, modId);
    if (version.empty())
        throw -1;
    else if (!ModManager::get().exists(modId, version)) {
        return version;
    }
    else {
#define PARAMETERS , modId, version, &d, path
        Wait2({
            auto mainArchivePath = ModManager::get().get_path(modId, version);
            auto newMainArchivePath = mainArchivePath + "2";

            ArchiveReader mainArchive(mainArchivePath);
            ArchiveReader archivePart(path);
            CustomArchive newArchive(newMainArchivePath, "");

            d.setValue(10);
            newArchive.clone(&mainArchive);
            d.setValue(60);
            newArchive.clone(&archivePart);
            d.setValue(90);

            std::filesystem::remove(mainArchivePath);
            std::filesystem::rename(newMainArchivePath, mainArchivePath);

        }, PARAMETERS);
        std::filesystem::remove(ModManager::get().get_log_path(modId, version));
        ModManager::get().mod_log(ModManager::get().get_path(modId, version), modId, version);
    }
    throw 1;
}
