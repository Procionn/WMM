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

ModInfo::ModInfo(const std::string& modversion, const unsigned long& localid) :
    localId(localid),
    modVersion(modversion)
{}


Mod::Mod (const std::string& modversion, const unsigned long& modid, const unsigned long& localid) :
          modId(modid) {
    versions = new std::vector<ModInfo>;
    versions->emplace_back(modversion, localid);
}
Mod::Mod (const unsigned long& modid) : modId(modid) {}
Mod::~Mod () {
    delete versions;
}
