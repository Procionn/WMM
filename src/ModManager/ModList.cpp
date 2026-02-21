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
#include "../patterns/WaitingWindow.h"
#include "../methods.h"
#include "../CONSTANTS.h"

namespace {
    bool modinfo_cmp(const ModInfo& a, const ModInfo& b) {
        return a.modVersion < b.modVersion;
    }

    bool mod_cmp(const Mod* a, const Mod* b) {
        return a->modId < b->modId;
    }
}



ModList::~ModList () {
    for (Mod* mod : list)
        delete mod;
    delete dataSaveFile;
}


Mod* ModList::bsearch (const uint64_t& modId) {
    auto it = std::lower_bound(list.begin(), list.end(), modId,
        [](const Mod* mod, uint64_t id) {return mod->modId < id;});
    if (it != list.end() && (*it)->modId == modId)
        return *it;
    return nullptr;
}


ModInfo* ModList::bsearch (Mod* ptr, const std::string& modVersion) {
    if (!ptr || !ptr->versions)
        std::runtime_error("ptr or ptr->versions is not valid");
    auto it = std::lower_bound(
        ptr->versions->begin(), ptr->versions->end(), modVersion,
        [](const ModInfo& info, const std::string& version)
        {return info.modVersion < version;});
    if (it != ptr->versions->end() && it->modVersion == modVersion)
        return &(*it);
    return nullptr;
}


void ModList::import_saved_data () {
    if (Core::CONFIG_GAME == "None")
        return;
    for (Mod* mod : list)
        delete mod;
    if (!list.empty())
        list.clear();
    if (std::filesystem::exists(saveFile)) {
        dataSaveFile = new wmml(saveFile);
        std::vector<wmml::variant> v(gridSize);
        list.reserve(dataSaveFile->height());
        while(dataSaveFile->read(v)) {
            add_in_ram(static_cast<void*>(&v));
            ++localId;
        }
        delete dataSaveFile;
        dataSaveFile = new wmml(saveFile);
    }
    else {
        std::filesystem::create_directories(std::filesystem::path(saveFile).parent_path());
        dataSaveFile = new wmml(saveFile, gridSize);
        dataSaveFile->flush();
    }
}


void ModList::add (const uint64_t& modId, const std::string modVersion, const std::string modName) {
    add_in_ram(modId, modVersion, modName);
    add_in_rom(modId, modVersion, modName);
    ++localId;
}


void ModList::add_in_ram(const uint64_t& modId, const std::string& modVersion,
                         const std::string& modName, const signed char type) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr)
            throw Core::lang["LANG_LABEL_MOD_EXISTS"];
        else {
            switch(type){
                case 0:
                    ptr->versions->emplace_back(modVersion, localId);
                    break;
                case 1:
                    ptr->add_cortege(modVersion, localId);
                    break;
            }
            std::sort(ptr->versions->begin(), ptr->versions->end(), modinfo_cmp);
        }
    }
    else {
        switch(type) {
            case 0:
                list.push_back(new Mod(modVersion, modId, localId));
                break;
            case 1:
                list.push_back(new Mod(modVersion, modId, localId, true));
                break;
        }
        std::sort(list.begin(), list.end(), mod_cmp);
    }
    dictionary[modName] = modId;
    reverceDictionary[modId] = modName;
}


void ModList::add_in_rom(const uint64_t& modId, const std::string& modVersion, const std::string& modName) {
    if (dataSaveFile) {
        std::vector<wmml::variant> v{modName, modVersion, modId, (signed char)(0)};
        dataSaveFile->write(v);
    }
    else
        std::runtime_error("BD file is not open");
}


void ModList::add_in_ram (const void* v) {
    const auto* c = static_cast<const std::vector<wmml::variant>*>(v);
    add_in_ram(std::get<uint64_t>(c->at(2)), std::get<std::string>(c->at(1)),
               std::get<std::string>(c->at(0)), std::get<signed char>(c->at(3)));
}


void ModList::create_cortege_in_ram (const std::vector<std::string>& versionsList, const std::string& name,
                                     const uint64_t modid) {
    Mod* ptr = bsearch(modid);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, name);
        if (version_ptr)
            throw Core::lang["LANG_LABEL_CORTEGE_EXISTS"];
        else {
            for (auto& entry: versionsList) {
                if (ModManager::get().is_cortege(modid, entry))
                    throw Core::lang["LANG_LABEL_MOD_IS_CORTEGE"];
            }
            ptr->versions->emplace_back(ModCortege(versionsList, name, localId));
            std::sort(ptr->versions->begin(), ptr->versions->end(), modinfo_cmp);
        }
    }
    else
        stc::cerr("Error. Trying to create a cortege before creating the mod structure!");
}


void ModList::create_cortege_in_rom (const std::vector<std::string>& versionsList, const std::string& name,
                                     const uint64_t modid) {
    if (dataSaveFile) {
        std::vector<wmml::variant> v{ModManager::get().mod_data_converter(modid), name, modid, (signed char)(1)};
        dataSaveFile->write(v);
    }
    else
        std::runtime_error("BD file is not open");

    std::ofstream file(stc::cwmm::cortege_path(name, modid));
    for (auto& entry : versionsList)
        file << entry << "\n";
}


void ModList::create_cortege (const std::vector<std::string>& versionsList, const std::string& name,
                              const uint64_t modid) {
    create_cortege_in_ram(versionsList, name, modid);
    create_cortege_in_rom(versionsList, name, modid);
    ++localId;
}



void ModList::ML_rom_remove (const uint64_t& localid) {
    dataSaveFile->remove_object(localid);
    for (const Mod* mod : list) {
        for(ModInfo& entry : *mod->versions) {
            if (entry.localId > localid)
                --entry.localId;
        }
    }
    --localId;
}


void ModList::ML_remove (const uint64_t& modId, const std::string& modVersion) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr) {
            auto index = version_ptr - ptr->versions->data(); // счёт от нуля
            ML_rom_remove(ptr->versions->data()[index].localId);
            ptr->versions->erase(ptr->versions->begin() + index);
            if (ptr->versions->empty()) {
                // в таком случае нужно уничтожить весь Mod объект
                auto iterator = std::find(list.begin(), list.end(), ptr);
                if (iterator != list.end()) {
                    list.erase(iterator);
                    delete ptr;
                }
                else
                    std::runtime_error(std::string("an object with an id ") + std::to_string(modId) + " not found");
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
        auto iterator = std::find(list.begin(), list.end(), ptr);
        if (iterator == list.end())
            std::runtime_error(std::string("an object with an id ") + std::to_string(modId) + " not found");
        list.erase(iterator);

        int i = 0;
        for (const ModInfo& entry : *ptr->versions) {
            ML_rom_remove(entry.localId - i);
            ++i;
        }
        delete ptr;
    }
    else
        throw ("Target mod is not exists");
}


const std::vector<Mod*>& ModList::all_mods_list() {
   return list;
}


const std::vector<std::string_view> ModList::all_versions_list (const uint64_t& modId) {
    auto* mod = bsearch(modId);
    assert(mod->versions);
    std::vector<std::string_view> versionList;
    versionList.reserve(mod->versions->size());
    for (const auto& entry : *mod->versions)
        versionList.emplace_back(entry.modVersion);
    return versionList;
}


void ModList::add_in_cortege (const uint64_t modId, const std::string& crtName,
                              const std::string& modVersion) {
    auto* ptr = bsearch(modId);
    if (ptr) {
        auto* obj = bsearch(ptr, crtName);
        if (obj) {
            if (ModManager::get().is_cortege(modId, crtName))
                static_cast<ModCortege*>(obj)->add(modVersion, modId);
            else
                throw std::runtime_error("object is not cortege");
        }
    }
    else
        throw std::runtime_error("mod is not exists");
}
