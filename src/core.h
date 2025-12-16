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
#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <map>
#include "core/IGameConfig.h"
class wmml;



class CBaseConfig
{
protected:
    bool configRead(std::istream& input, std::string& firstReturned, std::string& lastReturned);
};





class Lang : public virtual CBaseConfig
{
public:
    inline static std::map<std::string, std::string> lang;
    void update_lang();

protected:
    Lang();
};





class CConfigs : public virtual CBaseConfig
{
protected:
    CConfigs();

public:
    inline static std::string CONFIG_LANGUAGES;
    inline static std::string CONFIG_GAME;

    void config_reader();
    void overwriting_config_data();
};





class CBaseGameConfig : public virtual CBaseConfig
{
    friend class NixGameConfig;
    friend class WinGameConfig;
    static constexpr const char* config_string[4]{
        "ModCoreDirectoryStage",
        "OnlyModDirectory",
        "MixedGameDirectory",
        "URLtemplate"
    };
    std::vector<std::string> OMD; // Only Mods Directory
    std::vector<std::string> MGD; // Mixed Game Directory
    std::string core_dir_name;
    unsigned int GAME_CORE_DIR_STAGE;

public:
    inline static std::string CONFIG_GAME_PATH;
    inline static std::string CONFIG_EXECUTABLE_FILE;
    inline static std::string CONFIG_URL;
    
    void update_data_from_file();
    void save_game_path(const std::string& path);
    void game_dir_backup();
    void game_recovery();

    void restorer();
    virtual void symlink_deliting() = 0;
    virtual void symlink_creating(const std::string& targetCollection) = 0;

protected:
    CBaseGameConfig ();
    virtual void dir_comparison(const std::filesystem::path& file) = 0;

private:
    const int wmml_size = 3;
    void write(wmml* input, std::string str);
};





class CGameConfig : public CBaseGameConfig
{
    IGameConfig* object;

protected:
    CGameConfig() {
        /* object = new Core::configs["WMM_CONFIG_USE_EXTERNAL_MODULE"] == "true"
                     ? WinGameConfig : NixGameConfig; */
#ifdef WIN64
    object = new WinGameConfig;
#else
    object = new NixGameConfig;
#endif
    }
    ~CGameConfig() {
        delete object;
    }
    void dir_comparison (const std::filesystem::path& file) override {
        object->dir_comparison(file);
    }

public:
    void symlink_deliting() override {
        object->symlink_deliting();
    }
    void symlink_creating(const std::string& targetCollection) override {
        object->symlink_creating(targetCollection);
    }
};





class Core final : public CConfigs, public Lang, public CGameConfig
{
    Core() = default;
    ~Core() = default;
    friend class import;


    Core(Core&& ref) = delete;
    Core& operator=(Core&& other) = delete;
    Core(const Core& ref) = delete;
    Core& operator=(const Core& other) = delete;

protected:
    struct wmmb
    {
        uint64_t id;
        std::string version;
        std::string name;
        bool status = true;

        wmmb(void* v)  noexcept;
        bool operator==(const wmmb& last) const noexcept;
    };
    std::vector<Core::wmmb> parser(const std::filesystem::path& file,
                                   std::vector<std::string>* presets = nullptr,
                                   bool except = true);
    void compiller      (const std::vector<wmmb>& list,
                         const std::filesystem::path& directory);
    void optimizations  (std::vector<wmmb>& mainList,
                         std::vector<wmmb>& oldstruct);
    void clearing       (const std::vector<wmmb>& oldstruct,
                         const std::filesystem::path& directory);
    void collection_info(const std::vector<wmmb>& newstruct,
                         const std::filesystem::path& path,
                         const std::string& name);

public:
    static Core& get();

    void exporter(const std::string& name, const bool monolith);
    void importer(const std::string& path);

    void collector(const std::filesystem::path& name, bool type);

    struct CollectionInfo
    {
        uint64_t mods = 0, presets = 0, allMods = 0;
        CollectionInfo(const std::filesystem::path& name);
    };
};

#endif // CORE_H
