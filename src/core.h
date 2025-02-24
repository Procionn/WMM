#ifndef CORE_H
#define CORE_H

#include "wmml.h"

#include <vector>
#include <string>

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
public:
    std::vector<std::string> OMD;
    std::vector<std::string> MGD;
    std::string core_dir_name;
    int GAME_CORE_DIR_STAGE;
    std::string config_game_path;
    std::string config_executable_file;
    
    
    CGameConfig ();
    void game_path(std::string path);
    void game_dir_backup();
    void symlink_deliting();
    void dir_comparison(std::string& file);
    void symlink_creating(std::string& targetCollection);
private:
    int size = 3;
    void write(wmml& input, std::string str);
};




namespace configurator {
    struct wmmb
    {
        int id;
        std::string version;
        std::string name;
        bool status = true;
        
        wmmb(std::vector<std::string>& v);
    };
    bool operator==(wmmb& first, wmmb& last);
    
    std::vector<configurator::wmmb*> parser(std::string& file, int& publicCounter);
    void collector(std::string name, bool type);
    void compiller(std::string file, std::string directory);
}

void replace (std::string& input, char replaceable, char target);


#endif // CORE_H
