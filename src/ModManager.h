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
#include <map>
#include <tuple>
#include <stdint.h>

struct ModInfo
{
    ModInfo(const std::string& modVersion, const uint64_t& localId);
    virtual ~ModInfo() = default;

    bool isModInfo = true;
    uint64_t localId;
    std::string modVersion;
};



struct ModCortege : public ModInfo
{
    ModCortege(const std::vector<std::string>& versionsList, const std::string& name,
               const uint64_t& localId);
    ModCortege(const std::vector<std::string>&& versionsList, const std::string& name,
               const uint64_t& localId);

    void add(const std::string& version, const uint64_t modId);
    std::vector<std::string> dependence;
};



struct Mod
{
    Mod(const uint64_t& modId);
    Mod(const std::string& modVersion, const uint64_t& modId, const uint64_t& localId);
    Mod(const std::string& modVersion, const uint64_t& modId, const uint64_t& localId, bool);
    ~Mod();
    Mod(Mod&& ref) noexcept;
    Mod& operator=(Mod&& other) noexcept;

    void add_cortege(const std::string& name, const uint64_t& localid);

    std::vector<ModInfo>* versions = nullptr;
    uint64_t modId;

    std::string recommended_version() const;
};



class ModList
{
    std::vector<Mod*> list;
    uint64_t localId = 0;
    static constexpr const unsigned char gridSize = 4;

    ModList(ModList&& ref) = delete;
    ModList& operator=(ModList&& other) = delete;
    ModList(const ModList& ref) = delete;
    ModList& operator=(const ModList& other) = delete;

protected:
    std::string saveFile;
    std::map<std::string, uint64_t> dictionary;
    std::map<uint64_t, std::string> reverceDictionary;
    class wmml* dataSaveFile = nullptr;

private:
    void add_in_ram(const void*);
    void create_object_in_ram(const std::string&, const uint64_t&, signed char type);

protected:
    ModList() = default;
    virtual ~ModList();
    Mod*     bsearch(const uint64_t& modId);
    ModInfo* bsearch(Mod* ptr, const std::string& modVersion);
    void add_in_ram(const uint64_t& modId, const std::string& modVersion,
                    const std::string& modName, const signed char type = 0);
    void add_in_rom(const uint64_t& modId, const std::string& modVersion,
                    const std::string& modName);
    void ML_remove(const uint64_t& modId, const std::string& modVersion);
    void ML_remove(const uint64_t& modId);
    void ML_rom_remove(const uint64_t& localId);
    std::string mod_archive_unificate(const std::string& path, const uint64_t& modId, Mod* ptr,
                                      const std::string& version, const std::string& name);
    void import_saved_data();
    void create_cortege_in_ram(const std::vector<std::string>& versionsList,
                               const std::string& name, const uint64_t modid);
    void create_cortege_in_rom(const std::vector<std::string>& versionsList,
                               const std::string& name, const uint64_t modid);

public:
    void add(const uint64_t& modId, const std::string modVersion, const std::string modName);
    void create_cortege(const std::vector<std::string>& versionsList, const std::string& name,
                        const uint64_t modid);
    void add_in_cortege(const uint64_t modId, const std::string& crtName,
                        const std::string& modVersion);
    const std::vector<Mod*>& all_mods_list();
    const std::vector<std::string_view> all_versions_list(const uint64_t& modId);
};



class ModManager final : public ModList
{
    friend class ModList;
    const std::string archiveExpansion = ".MOD";
    bool copy = true; // false to move new mods in directory

    std::tuple<std::string, uint64_t, std::string> regex(const std::string& path);
    std::string path();
    void mod_log(const std::string& archivePath, const uint64_t id, const std::string& version);
    ModManager();
    ~ModManager() = default;

public:
    static ModManager& get();
    void update();
    void flush(); // enforces changes made to the database file
    bool get_copy();
    void set_copy(const bool&);

    void load(const std::string& path);

    bool exists(const uint64_t id,  const std::string& version);
    bool exists(const std::string& name, const std::string& version);

    bool is_cortege(const uint64_t id,  const std::string& version);
    std::vector<ModInfo*> get_cortege_list(const uint64_t id,  const std::string& version);

    void remove(const uint64_t id);
    void remove(const uint64_t id,  const std::string& version);
    void remove(const std::string& name);
    void remove(const std::string& name, const std::string& version);

    std::string get_path(const uint64_t id);
    std::string get_path(const std::string& name);
    std::string get_path(const uint64_t id, const std::string& version);
    std::string get_path(const std::string& name, const std::string& version);

    std::string get_log_path(const uint64_t id, const std::string& version);
    std::string get_log_path(const std::string& name, const std::string& version);

    std::string get_cortege_path(const uint64_t id, const std::string& name);

    uint64_t    mod_data_converter(const std::string& modName);
    std::string mod_data_converter(const uint64_t modId);

    std::string mod_recommended_version(const uint64_t modId);
    std::string mod_recommended_version(const std::string& modName);
};

#endif // MODMANAGER_H
