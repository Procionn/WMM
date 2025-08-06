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
#ifndef MODMANAGER_H
#define MODMANAGER_H

#include <string>
#include <vector>
#include <wmml.h>
#include <map>

struct ModInfo
{
    ModInfo(const std::string& modVersion, const unsigned long& localId);

    unsigned long localId;
    std::string modVersion;
};





struct Mod
{
    Mod(const unsigned long& modId);
    Mod(const std::string& modVersion, const unsigned long& modId, const unsigned long& localId);
    ~Mod();

    std::vector<ModInfo>* versions = nullptr;
    unsigned long modId;

    std::string recommended_version();
};





class ModList
{
    std::vector<Mod> list;
    unsigned long long localId = 0;
    static constexpr const unsigned char gridSize = 3;

protected:
    std::string saveFile;
    std::map<std::string, unsigned long> dictionary;
    std::map<unsigned long, std::string> reverceDictionary;
    wmml* dataSaveFile = nullptr;

private:
    void add(const std::vector<wmml::variant>&);

protected:
    ModList();
    Mod*     bsearch(const unsigned long& modId);
    ModInfo* bsearch(Mod* ptr, const std::string& modVersion);
    void add(const unsigned long& modId, const std::string& modVersion, const std::string& modName);
    void ML_remove(const unsigned long& modId, const std::string& modVersion);
    void ML_remove(const unsigned long& modId);
    void import_saved_data();

public:
    const std::vector<Mod>& all_mods_list();
};





class ModManager final : public ModList
{
    bool copy = true; // false to move new mods in directory
    std::string archiveExpansion = ".MOD";

    void* regex(const std::string& path);
    std::string path();
    void update();
    void mod_log(const std::string& archivePath, const unsigned long id, const std::string& version);
    ModManager();
    ~ModManager() = default;

public:
    static ModManager& get();
    void load(const std::string& path);

    bool exists(const unsigned long id,  const std::string& version);
    bool exists(const std::string& name, const std::string& version);

    void remove(const unsigned long id);
    void remove(const unsigned long id,  const std::string& version);
    void remove(const std::string& name);
    void remove(const std::string& name, const std::string& version);

    std::string get_path(const unsigned long id);
    std::string get_path(const unsigned long id, const std::string& version);
    std::string get_log_path(const unsigned long id, const std::string& version);
    std::string get_path(const std::string& name);
    std::string get_path(const std::string& name, const std::string& version);
    std::string get_log_path(const std::string& name, const std::string& version);

    unsigned long mod_data_converter(const std::string& modName);
    std::string mod_data_converter(const unsigned long modId);

    std::string mod_recommended_version(const unsigned long modId);
    std::string mod_recommended_version(const std::string& modName);
};

#endif // MODMANAGER_H
