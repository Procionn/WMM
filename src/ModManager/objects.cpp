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
#include <cassert>

ModInfo::ModInfo(const std::string& modversion, const uint64_t& localid) :
    localId(localid),
    modVersion(modversion)
{}


Mod::Mod (const std::string& modversion, const uint64_t& modid, const uint64_t& localid) :
          modId(modid) {
    versions = new std::vector<ModInfo>;
    versions->emplace_back(modversion, localid);
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
    delete versions;
}

std::string Mod::recommended_version () {
    assert(versions);
    return versions->back().modVersion;
}
