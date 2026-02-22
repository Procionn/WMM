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
#include"../methods.h"
#include <cassert>
#include <fstream>
#include <algorithm>

namespace {
bool modinfo_cmp (const ModInfo* a, const ModInfo* b) {
    return a->modVersion < b->modVersion;
}}


ModInfo::ModInfo(const std::string& modversion, const uint64_t& localid) :
    localId(localid),
    modVersion(modversion)
{}



ModCortege::ModCortege (const std::vector<std::string>& versionsList, const std::string& name,
                       const uint64_t& localid) :
    ModInfo(name, localid), dependence(versionsList) {
    isModInfo = false;
}


ModCortege::ModCortege (const std::vector<std::string>&& versionsList, const std::string& name,
                       const uint64_t& localid) :
    ModInfo(name, localid), dependence(versionsList) {
    isModInfo = false;
}


void ModCortege::add (const std::string& version, const uint64_t modId) {
    if (std::find(dependence.begin(), dependence.end(), version) != dependence.end())
        std::runtime_error("element is already exists");
    std::ofstream file(stc::cwmm::cortege_path(modVersion, modId));
    dependence.emplace_back(version);
    file << version;
}



Mod::Mod (const std::string& modversion, const uint64_t& modid, const uint64_t& localid)
    : modId(modid) {
    versions = new std::vector<ModInfo*>;
    versions->emplace_back(new ModInfo(modversion, localid));
}

Mod::Mod (const std::string& modVersion, const uint64_t& modid, const uint64_t& localId, bool)
    : modId(modid) {
    versions = new std::vector<ModInfo*>;
    add_cortege(modVersion, localId);
}

Mod::Mod (const uint64_t& modid) : modId(modid) {}


Mod::Mod(Mod&& ref) noexcept : versions(ref.versions), modId(ref.modId) {
    ref.versions = nullptr;
}


Mod& Mod::operator=(Mod&& other) noexcept {
    if (this != &other) {
        delete versions;
        versions = other.versions;
        modId = other.modId;
        other.versions = nullptr;
    }
    return *this;
}


Mod::~Mod () {
    for (auto* entry : *versions)
        delete entry;
    delete versions;
}

std::string Mod::recommended_version () const {
    assert(versions);
    return versions->back()->modVersion;
}

void Mod::add_cortege (const std::string& name, const uint64_t& localid) {
    std::ifstream file(stc::cwmm::cortege_path(name, modId));
    std::string str;
    std::vector<std::string> list;
    while(std::getline(file, str))
        list.emplace_back(str);
    versions->emplace_back(new ModCortege(std::move(list), name, localid));
    std::sort(versions->begin(), versions->end(), modinfo_cmp);
}
