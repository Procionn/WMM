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
#include <map>
#include "core/IGameConfig.h"
#include "api/export.h"


class CBaseConfig
{
protected:
    bool configRead(std::ifstream& input, std::string& firstReturned, std::string& lastReturned);
    virtual ~CBaseConfig() = default;
};





class WMMCORE_EXPORT Lang : public virtual CBaseConfig
{
public:
    inline static std::map<std::string, std::string> lang;
    void update_lang();

protected:
    Lang();
};





class WMMCORE_EXPORT CConfigs : public virtual CBaseConfig
{
    std::ofstream config;

protected:
    CConfigs();

public:
    inline static std::map<std::string, std::string> configs;
#define CONFIG_LANGUAGES        configs["WMM_CONFIG_LANGUAGES"]
#define CONFIG_GAME             configs["WMM_CONFIG_GAME"]

    void config_reader();
    void overwriting_config_data();
    void set_default(const std::string& key, const std::string& value);
};





class WMMCORE_EXPORT CBaseGameConfig : public virtual CBaseConfig
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
    void write(class wmml* input, std::string str);
};





class WMMCORE_EXPORT CGameConfig : public CBaseGameConfig
{
    IGameConfig* object;

protected:
    CGameConfig();
    ~CGameConfig();
    void dir_comparison (const std::filesystem::path& file) override;

public:
    void symlink_deliting() override;
    void symlink_creating(const std::string& targetCollection) override;
};





class WMMCORE_EXPORT Core final : public CConfigs, public Lang, public CGameConfig
{
    Core();
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
        signed char priority = 0;

        wmmb(void* v)  noexcept;
        bool operator==(const wmmb& last) const noexcept;
    };
    std::vector<Core::wmmb> parser(const std::filesystem::path& file,
                                   std::vector<std::string>* presets = nullptr,
                                   bool except = true);
    void compiller      (std::vector<wmmb>& list,
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
