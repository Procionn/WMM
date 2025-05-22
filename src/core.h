#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <map>
#include <wmml.h>


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





class CGameConfig : public virtual CBaseConfig
{
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
    void symlink_deliting();
    void symlink_creating(const std::string& targetCollection);
protected:
    CGameConfig ();
    void dir_comparison(const std::filesystem::path& file);
private:
    const int wmml_size = 3;
    void write(wmml& input, std::string str);
};





class Core final : public CConfigs, public Lang, public CGameConfig
{
    Core() = default;
    ~Core() = default;

protected:
    struct wmmb
    {
        unsigned long int id;
        std::string version;
        std::string name;
        bool status = true;

        wmmb(std::vector<wmml::variant>& v);
        bool operator==(wmmb& last);
    };
    std::vector<Core::wmmb*> parser(std::filesystem::path& file, int& publicCounter);
    void compiller(const std::filesystem::path& file, const std::filesystem::path& directory);

public:
    static Core& get();

    void collector(const std::filesystem::path& name, bool type);
};

#endif // CORE_H
