#include "core.h"
#include "CONSTANTS.h"
#include "wmml.h"

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <QCoreApplication>
namespace fs = std::filesystem;

std::string CConfigs::CONFIG_LANGUAGES;
std::string CConfigs::CONFIG_ARCHIVE_ITERATION;
std::string CConfigs::CONFIG_GAME;
std::string CConfigs::CONFIG_GAME_PATH;
std::string CConfigs::CONFIG_EXECUTABLE_FILE;




bool CBaseConfig::configRead (std::istream& input, std::string& firstReturned, std::string& lastReturned) {
    std::string readedLine;
    if (std::getline(input, readedLine)) {
        size_t part = readedLine.find_last_of('=');
        firstReturned = readedLine.substr(0, part);
        lastReturned = readedLine.substr(part + 1);
        return true;
    }
    else return false;
}



void CConfigs::config_reader () {
    std::ifstream readedFile (CONFIG);
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
             if (parameter == "Languages")         CONFIG_LANGUAGES         = indicator;
        else if (parameter == "Archive_iteration") CONFIG_ARCHIVE_ITERATION = indicator;
        else if (parameter == "Game")              CONFIG_GAME              = indicator;
    }
    CGameConfig tmp;
    CONFIG_GAME_PATH = tmp.config_game_path;
    CONFIG_EXECUTABLE_FILE = tmp.config_executable_file;
    
    readedFile.close();
}

void CConfigs::config_test () {
    const fs::path path {CONFIG};
    if (fs::exists(path)) {
        config_reader();
    }
    else {
        std::ofstream config(CONFIG, std::ios::app);
        config << "Languages=lang/EN.ini" << "\n";
        config << "Archive_iteration=None" << "\n";
        config << "Game=None" << "\n";
        config.close();
        config_reader();
    }
}

void CConfigs::config_save () {
    std::ofstream file(CONFIG, std::ios::out);
    file << "Languages=" << CONFIG_LANGUAGES << "\n";
    file << "Archive_iteration=" << CONFIG_ARCHIVE_ITERATION << "\n";
    file << "Game=" << CONFIG_GAME << "\n";
    file.close();
}










CGameConfig::CGameConfig () {
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        std::vector<std::string> v(size);
        while (file.read(v)) {
            if (v[0] == CConfigs::CONFIG_GAME) break;
        }
        config_game_path = v[2];
        config_executable_file = v[1];
        size_t part = v[2].find_last_of('/');
        core_dir_name = v[2].substr(part + 1);
    }
    else {
        config_game_path = "";
        config_executable_file = "";
    }
    
    std::ifstream readedFile ((GAMES + CConfigs::CONFIG_GAME + EXPANSION3).c_str());
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
        if (parameter == "ModCoreDirectoryStage") GAME_CORE_DIR_STAGE = std::stoi(indicator);
        else if (parameter == "OnlyModDirectory") OMD.push_back(indicator);
        else if (parameter == "MixedGameDirectory") MGD.push_back(indicator);
    }
}

void CGameConfig::write(wmml& input, std::string str) {
    std::vector<std::string> v(size);
    v[0] = CConfigs::CONFIG_GAME;
    v[1] = str;
    for (int counter = GAME_CORE_DIR_STAGE; counter != 0; --counter) {
        size_t part = str.find_last_of('/');
        str = str.substr(0, part);
    }
    v[2] = str;
    input.add(v);
}

void CGameConfig::game_path (std::string path) {
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        int counter = 0;
        while (true) {
            std::string rf = file.read_field();
            if (rf == CConfigs::CONFIG_GAME){
                std::vector<std::string> v(size);
                v[0] = CConfigs::CONFIG_GAME;
                v[1] = path;
                for (int counter = GAME_CORE_DIR_STAGE; counter != 0; --counter) {
                    size_t part = path.find_last_of('/');
                    path = path.substr(0, part);
                }
                v[2] = path;
                file.replace(counter, v);
                break;
            }
            else if (!(file.skip(size - 1))) {
                write(file, path);
                break;
            }
            ++counter;
        }
    }
    else {
        wmml file(SAVE, size);
        write(file, path);
    }
    
}

void CGameConfig::game_dir_backup () {
    std::string targetpath = GAME + "/" + core_dir_name;
    if (fs::exists(targetpath)) fs::remove_all(targetpath);
    try {
        for (std::string p : MGD) {
            std::string pt = config_game_path + "/" + p;
            fs::path sorce_path(pt);
            fs::path target_path(targetpath);
            for (const auto& entry : fs::recursive_directory_iterator(sorce_path)) {
                if (fs::is_regular_file(entry.path())) {
                    if (!fs::exists(target_path)) {
                        fs::create_directories(target_path.parent_path());
                        fs::create_directories(target_path);
                    }
                    fs::path target_path_new(fs::relative(entry.path(), sorce_path));
                    target_path_new = target_path / p / target_path_new;
                    fs::create_directories(target_path_new.parent_path());
                    fs::copy_file(entry.path(), target_path_new);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::symlink_deliting () {
    std::string testFile = config_game_path + "/" + CONST_FILE + EXPANSION;
    if (fs::exists(testFile)) dir_comparison(testFile);
    try {
        for (const auto& entry : fs::recursive_directory_iterator(config_game_path)) {
            const auto& status = entry.symlink_status();
            if (fs::is_symlink(status)) fs::remove(entry);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::dir_comparison (std::string& file) {
    wmml targetFile(file);
    std::vector<std::string> v(GRID_WIDTH);
    while(targetFile.read(v))
        if (v[0] == "this") break;
    try {
        // targetpath       =  game/Cyberpunk 2077/
        // pt               =  C://Game/Cyberpunk 2077/[MGD]/
        // config_game_path =  C://Game/Cyberpunk 2077/
        
        std::string targetpath = GAME + "/" + core_dir_name;
        for (std::string p : MGD) {
            std::string pt = config_game_path + "/" + p;
            for (const auto& entry : fs::recursive_directory_iterator(pt)) {
                fs::path relative = fs::relative(entry.path(), config_game_path);
                fs::path target_path = targetpath / relative;
                fs::path backup_path = (COLLECTIONS + CConfigs::CONFIG_GAME + "/" + v[1]) / relative;
                if (!fs::exists(target_path)) fs::rename(entry.path(), backup_path);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::symlink_creating (std::string& targetCollection) {
    std::string collect = COLLECTIONS + CConfigs::CONFIG_GAME + "/" + targetCollection;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(collect)) {
            fs::path relative = fs::relative(entry.path(), collect);
            fs::path target_path = config_game_path / relative;
            fs::path current_dir = QCoreApplication::applicationDirPath().toStdString();
            fs::path global_target_path = current_dir / entry;
            if (fs::is_directory(entry)){
                fs::create_directories(target_path);
            }
            if (fs::exists(target_path)) {
                if (!fs::is_directory(target_path)) {
                    fs::remove(target_path);
                    fs::create_directories(target_path.parent_path());
                    fs::create_symlink(global_target_path, target_path);
                }
            }
            else {
                fs::create_directories(target_path.parent_path());
                fs::create_symlink(global_target_path, target_path);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}






configurator::wmmb::wmmb (std::vector<std::string>& v) {
    id = std::stoi(v[4]);
    version = v[2];
    name = v[1];
}

bool configurator::operator== (wmmb& first, wmmb& last) {
    if (first.id == last.id && first.version == last.version) return true;
    else return false;
}

void configurator::compiller (std::string file, std::string directory) {
    std::vector<std::string> v(GRID_WIDTH);
    fs::path fsDir = directory;
    wmml openedFile(file);
    while (openedFile.read(v)) {
        if (v[5] == "1") {
            if (v[3] == "mod") {
                std::string path = MODS + CConfigs::CONFIG_GAME + "/" + v[1];
                fs::path fsPath = path;
                for (const auto& entry : fs::recursive_directory_iterator(fsPath)) {
                    if (fs::is_regular_file(entry.path())) {
                        fs::path relative_path = fs::relative(entry.path(), fsPath);
                        fs::path target_file_path = fsDir / relative_path;
                        fs::create_directories(target_file_path.parent_path());
                        fs::copy_file(entry.path(), target_file_path, fs::copy_options::overwrite_existing);
                    }
                }
            }
            else {
                std::string nfile = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS + v[1] + EXPANSION;
                compiller(nfile, directory);
            }
        }
    }
}

std::vector<configurator::wmmb*> configurator::parser (std::string& file, int& publicCounter) {
    const int constSize = 256;
    int targetSize = constSize;
    publicCounter = 0;
    std::vector<configurator::wmmb*> presets(targetSize);
    std::vector<std::string> v(GRID_WIDTH);
    wmml targetfile(file);
    while (targetfile.read(v)) {
        if (publicCounter != targetSize) {
            if (v[3] == "this") continue;
            if (v[3] == "mod") {
                presets[publicCounter] = new configurator::wmmb(v);
                ++publicCounter;
            }
            else {
                std::string tmp = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS + v[1] + EXPANSION;
                wmml tmpFile(tmp);
                while(tmpFile.read(v)) {
                    assert(v[4] == "mod");
                    presets[publicCounter] = new configurator::wmmb(v);
                    ++publicCounter;
                }
            }
        }
        else {
            targetSize = targetSize + constSize;
            presets.resize(targetSize);
        }
    }
    return presets;
}

void configurator::collector(std::string name, bool type) {
    std::string file;
    std::string dir = COLLECTIONS + CConfigs::CONFIG_GAME + "/" + name;
    std::string oldFile = dir + "/" + CONST_FILE + EXPANSION;
    if (type) {
        std::cerr << "Exporting presets is not supported" << std::endl;
        abort();
    }
    else      file = RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS + name + EXPANSION;
    
    if (fs::exists(dir)) {
        int NFS;
        int OFS;
        std::vector<wmmb*> newstruct = parser(file, NFS);
        std::vector<wmmb*> oldstruct = parser(oldFile, OFS);
        for (int firstcounter = 0; firstcounter != OFS; ++firstcounter) {
            for (int lastcounter = 0; lastcounter != NFS; ++lastcounter) {
                if (*oldstruct[firstcounter] == *newstruct[lastcounter]) {
                    oldstruct[firstcounter]->status = false;
                    newstruct[lastcounter]->status = false;
                    break;
                }
            }
        }
        
        for (int counter = 0; counter != OFS; ++counter) {
            if (oldstruct[counter]) {
                std::string path = ARCHIVE + CConfigs::CONFIG_GAME + "/" + std::to_string(oldstruct[counter]->id) +
                                   oldstruct[counter]->version + EXPANSION2;
                std::string str;
                std::ifstream readedFile(path);
                while (std::getline(readedFile, str)) {
                    std::string deletedFile = COLLECTIONS + CConfigs::CONFIG_GAME + "/" + name + "/" + str;
                    fs::remove(deletedFile);
                }
            }
        }
        for (int counter = 0; counter != NFS; ++counter) {
            if (newstruct[counter]) {
                std::string path = MODS + CConfigs::CONFIG_GAME + "/" + newstruct[counter]->name;
                
                for (const auto& entry : fs::recursive_directory_iterator(path)) {
                    if (fs::is_regular_file(entry.path())) {
                        fs::path relative_path = fs::relative(entry.path(), path);
                        fs::path target_file_path = dir / relative_path;
                        fs::create_directories(target_file_path.parent_path());
                        fs::copy_file(entry.path(), target_file_path, fs::copy_options::overwrite_existing);
                    }
                }
            }
        }
        fs::path f1 = file;
        fs::path f2 = oldFile;
        fs::remove(f2);
        fs::copy_file(f1, f2);
        std::vector<std::string> v{"this", name, "this", "this", "this", "this"};
        wmml tmp(oldFile);
        tmp.add(v);
    }
    else {
        fs::create_directories(dir);
        compiller(file, dir);
        fs::path f1 = file;
        fs::path f2 = oldFile;
        fs::copy_file(f1, f2);
        std::vector<std::string> v{"this", name, "this", "this", "this", "this"};
        wmml tmp(oldFile);
        tmp.add(v);
    }
}
