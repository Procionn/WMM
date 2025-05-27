#include "../core.h"

#include "../methods.h"
#include "../CONSTANTS.h"

namespace fs = std::filesystem;

Core::wmmb::wmmb (std::vector<wmml::variant>& v) {
    id = std::get<unsigned long int>(v[3]);
    version = std::get<std::string>(v[1]);
    name = std::get<std::string>(v[0]);
}


bool Core::wmmb::operator== (wmmb& last) {
    if (this->id == last.id && this->version == last.version)
        return true;
    else return false;
}


void Core::compiller (const std::filesystem::path& file, const std::filesystem::path& directory) {
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
                        fs::copy_file(entry.path(), target_file_path,
                                      fs::copy_options::overwrite_existing);
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

std::vector<Core::wmmb*> Core::parser (const std::filesystem::path& file, int& publicCounter) {
    const int constSize = 256;
    int targetSize = constSize;
    publicCounter = 0;
    std::vector<Core::wmmb*> presets(targetSize);
    std::vector<wmml::variant> v(GRID_WIDTH);
    wmml targetfile(file);
    while (targetfile.read(v)) {
        if (publicCounter != targetSize) {
            if (std::get<std::string>(v[0]) == "this")
                continue;
            if (!std::get<bool>(v[2])) {
                presets[publicCounter] = new Core::wmmb(v);
                ++publicCounter;
            }
            else {
                std::string tmp = stc::cwmm::ram_preset(std::get<std::string>(v[1]));
                wmml tmpFile(tmp);
                while(tmpFile.read(v)) {
                    assert(std::get<bool>(v[2]));
                    presets[publicCounter] = new Core::wmmb(v);
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


void Core::collector(const std::filesystem::path& name, bool type) {
    // collected all mods file in directory
    fs::path directory = (COLLECTIONS + Core::CONFIG_GAME) / name;
    fs::path oldFile = directory / (CONST_FILE + EXPANSION);
    if (type)
        throw "Exporting presets is not supported";
    fs::path file = stc::cwmm::ram_collection(name.string());

    if (fs::exists(directory)) {
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
                std::string path = ARCHIVE + Core::CONFIG_GAME + "/" + std::to_string(oldstruct[counter]->id) +
                                   "/" + oldstruct[counter]->version + EXPANSION2;
                std::string str;
                std::ifstream readedFile(path);
                while (std::getline(readedFile, str)) {
                    fs::path deletedFile = (COLLECTIONS + Core::CONFIG_GAME) / name / str;
                    fs::remove(deletedFile);
                }
            }
        }
        for (int counter = 0; counter != NFS; ++counter) {
            if (newstruct[counter]) {
                std::string path = MODS + Core::CONFIG_GAME + "/" + newstruct[counter]->name;
                for (const auto& entry : fs::recursive_directory_iterator(path)) {
                    if (fs::is_regular_file(entry.path())) {
                        fs::path relative_path = fs::relative(entry.path(), path);
                        fs::path target_file_path = directory / relative_path;
                        fs::create_directories(target_file_path.parent_path());
                        fs::remove(target_file_path);
                        fs::copy_file(entry.path(), target_file_path,
                                      fs::copy_options::overwrite_existing);
                    }
                }
            }
        }
        fs::remove(oldFile);
        fs::copy_file(file, oldFile);
        std::vector<wmml::variant> v{"this", name.string(), false, 0, true};
        wmml tmp(oldFile);
        tmp.write(v);
    }
    else {
        fs::create_directories(directory);
        compiller(file, directory);
        fs::copy_file(file, oldFile);
        std::vector<wmml::variant> v{"this", name.string(), false, 0, true};
        wmml tmp(oldFile);
        tmp.write(v);
    }
}

