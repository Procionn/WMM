#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <QCoreApplication>
#include <wmml.h>

class CBaseConfig
{
public:
    bool configRead (std::istream& input, std::string& firstReturned, std::string& lastReturned);
};



class CConfigs : public CBaseConfig
{
public:
    static std::string CONFIG_LANGUAGES;
    static std::string CONFIG_ARCHIVE_ITERATION;
    static std::string CONFIG_GAME;
    static std::string CONFIG_GAME_VERSION;
    static std::string CONFIG_GAME_PATH;
    static std::string CONFIG_EXECUTABLE_FILE;
    
    void config_reader ();
    void config_test ();
    void config_save();
};





class CGameConfig : public CBaseConfig
{
    std::string config_string[4] {
        "ModCoreDirectoryStage",
        "OnlyModDirectory",
        "MixedGameDirectory",
        "URLtemplate"
    };
    std::vector<std::string> OMD; // Only Mods Directory
    std::vector<std::string> MGD; // Mixed Game Directory
    std::string core_dir_name;
    int GAME_CORE_DIR_STAGE;
public:
    std::string config_game_path;
    std::string config_executable_file;
    std::string config_url;
    
    
    CGameConfig ();
    void game_path(std::string path);
    void game_dir_backup();
    void symlink_deliting();
    void dir_comparison(std::filesystem::path& file);
    void symlink_creating(std::string& targetCollection);
    void game_recovery();
    void restorer();
private:
    int size = 3;
    void write(wmml& input, std::string str);
};




namespace configurator {
    struct wmmb
    {
        unsigned long int id;
        std::string version;
        std::string name;
        bool status = true;
        
        wmmb(std::vector<wmml::variant>& v);
    };
    bool operator==(wmmb& first, wmmb& last);
    
    std::vector<configurator::wmmb*> parser(std::filesystem::path& file, int& publicCounter);
    void collector(std::filesystem::path name, bool type);
    void compiller(std::filesystem::path& file, std::filesystem::path& directory);
}

void replace (std::string& input, char replaceable, char target);


#endif // CORE_H
