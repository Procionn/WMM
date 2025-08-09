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


Mod* ModList::bsearch (const unsigned long& modId) {
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
    if (!list.empty())
        list.clear();
    if (std::filesystem::exists(saveFile)) {
        dataSaveFile = new wmml(saveFile);
        std::vector<wmml::variant> v(gridSize);
        list.reserve(dataSaveFile->height());
        while(dataSaveFile->read(v))
            add_in_ram(v);
        delete dataSaveFile;
        dataSaveFile = new wmml(saveFile);
    }
    else {
        std::filesystem::create_directories(std::filesystem::path(saveFile).parent_path());
        dataSaveFile = new wmml(saveFile, gridSize);
    }
}


void ModList::add (const unsigned long& modId, const std::string& modVersion, const std::string& modName) {
    add_in_ram(modId, modVersion, modName);
    add_in_rom(modId, modVersion, modName);
    ++localId;

}


void ModList::add_in_ram(const unsigned long& modId, const std::string& modVersion, const std::string& modName) {
    Mod* ptr = bsearch(modId);
    if (ptr) {
        auto* version_ptr = bsearch(ptr, modVersion);
        if (version_ptr)
            throw ("The added mod is already exists");
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


void ModList::add_in_rom(const unsigned long& modId, const std::string& modVersion, const std::string& modName) {
    if (dataSaveFile) {
        std::vector<wmml::variant> v{modName, modVersion, modId};
        dataSaveFile->write(v);
    }
    else
        std::runtime_error("BD file is not open");
}


void ModList::add_in_ram (const std::vector<wmml::variant>& v) {
    add_in_ram(std::get<unsigned long>(v[2]), std::get<std::string>(v[1]), std::get<std::string>(v[0]));
}


void ModList::ML_remove (const unsigned long& modId, const std::string& modVersion) {
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


void ModList::ML_remove (const unsigned long& modId) {
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
