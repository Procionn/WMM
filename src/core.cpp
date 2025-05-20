#include "core.h"
#include "CONSTANTS.h"
#include "wmml.h"
#include "methods.h"

#ifdef _WIN32
    #include <windows.h>
    #include <sys/stat.h>
#endif
#ifndef NDEBUG
    #include <iomanip>
#endif

namespace fs = std::filesystem;

std::string CConfigs::CONFIG_LANGUAGES;
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
        config << "Game=None" << "\n";
        config.close();
        config_reader();
    }
}

void CConfigs::config_save () {
    std::ofstream file(CONFIG, std::ios::out);
    file << "Languages=" << CONFIG_LANGUAGES << "\n";
    file << "Game=" << CONFIG_GAME << "\n";
    file.close();
}










CGameConfig::CGameConfig () {
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        std::vector<wmml::variant> v(size);
        while (file.read(v)) {
            if (std::get<std::string>(v[0]) == CConfigs::CONFIG_GAME)
                break;
        }
        config_game_path = std::get<std::string>(v[2]);
        config_executable_file = std::get<std::string>(v[1]);
        size_t part = config_game_path.find_last_of('/');
        core_dir_name = config_game_path.substr(part + 1);
    }
    else {
        config_game_path = "";
        config_executable_file = "";
    }
    
    std::ifstream readedFile ((GAMES + CConfigs::CONFIG_GAME + EXPANSION3).c_str());
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
             if (parameter == config_string[0]) GAME_CORE_DIR_STAGE = std::stoi(indicator);
        else if (parameter == config_string[1]) OMD.push_back(indicator);
        else if (parameter == config_string[2]) MGD.push_back(indicator);
        else if (parameter == config_string[3]) config_url =  indicator;
    }
}

void CGameConfig::write(wmml& input, std::string str) {
    std::vector<wmml::variant> v(size);
    v[0] = CConfigs::CONFIG_GAME;
    v[1] = str;
    for (int counter = GAME_CORE_DIR_STAGE; counter != 0; --counter) {
        size_t part = str.find_last_of('/');
        str = str.substr(0, part);
    }
    v[2] = str;
    input.write(v);
}

void CGameConfig::game_path (std::string path) {
    // create a save of game in save.wmml file
    if (fs::exists(SAVE)) {
        wmml file(SAVE);
        std::vector<wmml::variant> v(size);
        for (int counter = 0; file.read(v); ++counter) {
            if (std::get<std::string>(v[0]) == CConfigs::CONFIG_GAME) {
                file.remove_object(counter);
                write(file, path);
            }
        }
    }
    else {
        wmml file(SAVE, size);
        write(file, path);
    }
}

void CGameConfig::game_dir_backup () {
    std::string targetpath = GAME + core_dir_name;
    if (fs::exists(targetpath)) stc::fs::remove_all(targetpath);
    try {
        for (std::string& p : MGD) {
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
                    fs::copy_file(stc::string::replace(entry.path(), '\\', '/'), stc::string::replace(target_path_new, '\\', '/'));
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::symlink_deliting () {
    std::string string =  config_game_path + "/" + CONST_FILE + EXPANSION;
    fs::path testFile = string;
    std::cout << testFile << std::endl;
    if (fs::exists(testFile)) dir_comparison(testFile);
    try {
#ifdef _WIN32
        auto is_symlink = [](const std::filesystem::path& p) -> bool {
            DWORD attrs = GetFileAttributesA(p.string().c_str());
            return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
        };

        for (const auto& entry : fs::recursive_directory_iterator(config_game_path)) {
            if (is_symlink(entry.path())) {
                DeleteFileA(stc::string::replace(entry.path(), '\\', '/').string().c_str());
            }
        }
#elif defined(__linux__)
        for (const auto& entry : fs::recursive_directory_iterator(config_game_path)) {
            const auto& status = entry.symlink_status();
            if (fs::is_symlink(status)) {
                std::cout << entry.path() << " is symlink" << std::endl;
                fs::remove(entry);
            }
        }
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::dir_comparison (std::filesystem::path& file) {
    wmml targetFile(file);
    std::vector<wmml::variant> v(GRID_WIDTH);
    while(targetFile.read(v))
        if (std::get<std::string>(v[0]) == "this") break;
    try {
        // targetpath       =  game/Cyberpunk 2077/
        // pt               =  C://Game/Cyberpunk 2077/[MGD]/
        // config_game_path =  C://Game/Cyberpunk 2077/
        
        std::string targetpath = GAME + "/" + core_dir_name;
        for (const std::string& p : MGD) {
            std::string pt = config_game_path + "/" + p;
            for (const auto& entry : fs::recursive_directory_iterator(pt)) {
                fs::path relative = fs::relative(entry.path(), config_game_path);
                fs::path target_path = targetpath / relative;
                fs::path backup_path = (COLLECTIONS + CConfigs::CONFIG_GAME + "/" + std::get<std::string>(v[0])) / relative;
                if (!fs::exists(target_path)) {
#ifdef _WIN32
                    auto is_symlink = [](const std::filesystem::path& p) -> bool {
                        DWORD attrs = GetFileAttributesA(p.string().c_str());
                        return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT);
                    };

                    if (is_symlink(entry.path()))
                        continue;
                    struct stat info;
                    if (stat(entry.path().string().c_str(), &info) == 0 && S_ISDIR(info.st_mode))
                        continue;

                    fs::create_directories(backup_path.parent_path());
                    if (fs::exists(backup_path))
                        fs::remove(backup_path);
                    fs::rename(stc::string::replace(entry.path(), '\\', '/'), stc::string::replace(backup_path, '\\', '/'));
#elif defined(__linux__)
                    if (fs::is_directory(entry.path()) || fs::is_symlink(entry.path()))
                        continue;
                    fs::rename(stc::string::replace(entry.path(), '\\', '/'), stc::string::replace(backup_path, '\\', '/'));
#endif
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::symlink_creating (std::string& targetCollection) {
    restorer();
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
                    stc::fs::symlink(global_target_path, target_path);
                }
            }
            else stc::fs::symlink(global_target_path, target_path);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void CGameConfig::game_recovery () {
    symlink_deliting();

    auto deleteig_cycle = [=](std::vector<std::string>& vector) -> void {
        for (std::string& target : vector) {
            std::string path = config_game_path + "/" + target;
            if (!fs::exists(path))
                continue;
            stc::fs::remove_all(path);
        }
    };

    deleteig_cycle(OMD);
    deleteig_cycle(MGD);
    std::string backup = stc::cwmm::backup_path();
    for (const auto& entry : fs::recursive_directory_iterator(backup)) {
        if (fs::is_directory(entry.path()))
            continue;
        fs::path relative = fs::relative(entry.path(), backup);
        fs::path path = config_game_path + "/" + relative.string();
        fs::create_directories(path.parent_path());
        fs::copy(entry.path(), path);
    }
}

void CGameConfig::restorer () {
    for (const auto& entry : fs::recursive_directory_iterator(stc::cwmm::backup_path())) {
        fs::path relative = fs::relative(entry.path(), stc::cwmm::backup_path());
        fs::path target = config_game_path / relative;
        if (!fs::exists(target)) {
            fs::path global_backup = QCoreApplication::applicationDirPath().toStdString() / entry.path();
#ifndef NDEBUG
            std::cout << std::left << std::setw(100)
                      << global_backup << " -> " << target << std::endl;
#endif
            fs::copy(global_backup, target);
        }

    }
}






configurator::wmmb::wmmb (std::vector<wmml::variant>& v) {
    id = std::get<unsigned long int>(v[3]);
    version = std::get<std::string>(v[1]);
    name = std::get<std::string>(v[0]);
}

bool configurator::operator== (wmmb& first, wmmb& last) {
    if (first.id == last.id && first.version == last.version)
        return true;
    else return false;
}

void configurator::compiller (std::filesystem::path& file, std::filesystem::path& directory) {
    std::vector<wmml::variant> v(GRID_WIDTH);
    wmml openedFile(file);
    while (openedFile.read(v)) {
        if (std::get<bool>(v[4])) {
            if (!std::get<bool>(v[2])) {
                std::string path = stc::cwmm::ram_mods(std::get<std::string>(v[0]));
                fs::path fsPath = path;
                for (const auto& entry : fs::recursive_directory_iterator(fsPath)) {
                    if (fs::is_regular_file(entry.path())) {
                        fs::path relative_path = fs::relative(entry.path(), fsPath);
                        fs::path target_file_path = directory / relative_path;
                        fs::create_directories(target_file_path.parent_path());
                        fs::copy_file(entry.path(), target_file_path, fs::copy_options::overwrite_existing);
                    }
                }
            }
            else {
                fs::path nfile = stc::cwmm::ram_preset(std::get<std::string>(v[0]));
                compiller(nfile, directory);
            }
        }
    }
}

std::vector<configurator::wmmb*> configurator::parser (std::filesystem::path& file, int& publicCounter) {
    const int constSize = 256;
    int targetSize = constSize;
    publicCounter = 0;
    std::vector<configurator::wmmb*> presets(targetSize);
    std::vector<wmml::variant> v(GRID_WIDTH);
    wmml targetfile(file);
    while (targetfile.read(v)) {
        if (publicCounter != targetSize) {
            if (std::get<std::string>(v[0]) == "this")
                continue;
            if (!std::get<bool>(v[2])) {
                presets[publicCounter] = new configurator::wmmb(v);
                ++publicCounter;
            }
            else {
                std::string tmp = stc::cwmm::ram_preset(std::get<std::string>(v[1]));
                wmml tmpFile(tmp);
                while(tmpFile.read(v)) {
                    assert(std::get<bool>(v[2]));
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

void configurator::collector(std::filesystem::path name, bool type) {
    // collected all mods file in directory
    fs::path fsDir = (COLLECTIONS + CConfigs::CONFIG_GAME) / name;
    fs::path oldFile = fsDir / (CONST_FILE + EXPANSION);
    if (type) {
        std::cerr << "Exporting presets is not supported" << std::endl;
        abort();
    }
    fs::path file = stc::cwmm::ram_collection(name.string());
    
    if (fs::exists(fsDir)) {
        int NFS;
        int OFS;
        std::vector<wmmb*> newstruct = parser(file, NFS);
        std::vector<wmmb*> oldstruct = parser(oldFile, OFS);
        // Collection optimization cycle. Checks if the mod was available in the previous collection iteration
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
                                   "/" + oldstruct[counter]->version + EXPANSION2;
                std::string str;
                std::ifstream readedFile(path);
                while (std::getline(readedFile, str)) {
                    fs::path deletedFile = (COLLECTIONS + CConfigs::CONFIG_GAME) / name / str;
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
                        fs::path target_file_path = fsDir / relative_path;
                        fs::create_directories(target_file_path.parent_path());
                        fs::remove(target_file_path);
                        fs::copy_file(entry.path(), target_file_path, fs::copy_options::overwrite_existing);
                    }
                }
            }
        }
        fs::path f1 = file;
        fs::path f2 = oldFile;
        fs::remove(f2);
        fs::copy_file(f1, f2);
        std::vector<wmml::variant> v{"this", name.string(), false, 0, true};
        wmml tmp(oldFile);
        tmp.write(v);
    }
    else {
        fs::create_directories(fsDir);
        compiller(file, fsDir);
        fs::path f1 = file;
        fs::path f2 = oldFile;
        fs::copy_file(f1, f2);
        std::vector<wmml::variant> v{"this", name.string(), false, 0, true};
        wmml tmp(oldFile);
        tmp.write(v);
    }
}

