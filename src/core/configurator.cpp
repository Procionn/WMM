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
#include "../core.h"

#include <hpp-archive.h>
#include "../methods.h"
#include "../CONSTANTS.h"
#include "../patterns/ERRORdialog.h"
#include "../patterns/RenameDialog.h"
#include "../importer.h"

namespace fs = std::filesystem;

Core::wmmb::wmmb (std::vector<wmml::variant>& v) noexcept {
    assert(std::get<bool>(v[4]));
    id = std::get<unsigned long int>(v[3]);
    assert(std::get<bool>(v[2]));
    version = std::get<std::string>(v[1]);
    name = std::get<std::string>(v[0]);
}


bool Core::wmmb::operator== (const wmmb& last) const noexcept{
    if (this->id == last.id && this->version == last.version)
        return true;
    else return false;
}


std::vector<Core::wmmb> Core::parser (const std::filesystem::path& file, std::vector<std::string>* presets) {
    // Recursively processes the collection file, producing a monotonous vector of mods at the output
    if (presets && presets->empty())
        presets->reserve(20);

    std::vector<Core::wmmb> list;
    list.reserve(300);
    std::vector<wmml::variant> v(GRID_WIDTH);
    wmml targetfile(file);
    while (targetfile.read(v)) {
        if (std::get<std::string>(v[1]) == "this")
            continue;
        if (std::get<bool>(v[2])) {
            if (!std::filesystem::exists(stc::cwmm::ram_mods(std::get<std::string>(v[0]))))
                throw (Core::lang["LANG_LABEL_NOT_EXIST_OBJECT"] + " mod " + std::get<std::string>(v[0]));
            list.emplace_back(v);
        }
        else {
            if (!std::filesystem::exists(stc::cwmm::ram_preset(std::get<std::string>(v[0]))))
                throw (Core::lang["LANG_LABEL_NOT_EXIST_OBJECT"] + " preset " + std::get<std::string>(v[0]));
            if (presets)
                presets->emplace_back(std::get<std::string>(v[0]));
            wmml file(stc::cwmm::ram_preset(std::get<std::string>(v[0])));
            while(file.read(v)) {
                assert(std::get<bool>(v[2]));
                if (!std::filesystem::exists(stc::cwmm::ram_mods(std::get<std::string>(v[0]))))
                    throw (Core::lang["LANG_LABEL_NOT_EXIST_OBJECT"] + " mod " + std::get<std::string>(v[0]));
                list.emplace_back(v);
            }
        }
    }
    return list;
}


void Core::compiller (const std::vector<wmmb>& list, const std::filesystem::path& directory) {
    // Using a monotone vector of mods, it collects their files into a directory
    for (const auto& obj : list) {
        if (obj.status) {
            fs::path path = stc::cwmm::ram_mods(obj.name);
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
}


void Core::optimizations (std::vector<wmmb>& mainList, std::vector<wmmb>& oldstruct) {
    // Collection optimization cycle. Checks if the mod was available in the previous collection iteration
    for (auto& oldObject : oldstruct) {
        for (auto& newObject : mainList) {
            if (oldObject == newObject) {
                oldObject.status = false;
                newObject.status = false;
                break;
            }
        }
    }
}


void Core::clearing (const std::vector<wmmb>& oldstruct, const std::filesystem::path& directory) {
    // deleting the files of old mods
    for (const auto& mod : oldstruct) {
        fs::path path = (ARCHIVE + Core::CONFIG_GAME) / (fs::path)std::to_string(mod.id) / (mod.version + EXPANSION2);
        std::string str;
        std::ifstream readedFile(path);
        while (std::getline(readedFile, str)) {
            fs::path deletedFile = directory / str;
            fs::remove(deletedFile);
        }
    }
}


void Core::collection_info(const std::vector<wmmb>& newstruct, const std::filesystem::path& path, const std::string& name) {
    wmml file(path, GRID_WIDTH);
    std::vector<wmml::variant> v(GRID_WIDTH);

    for (const auto& ptr : newstruct) {
        v = {ptr.name, ptr.version, true, ptr.id, true};
        file.write(v);
    }
    v = {name, "this", true, 0, true};
    file.write(v);
}


void Core::collector(const std::filesystem::path& name, bool type) {
    // collected all mods file in directory
    try {
        fs::path directory = (COLLECTIONS + Core::CONFIG_GAME) / name;
        fs::path oldFile = directory / (CONST_FILE + EXPANSION);
        if (type)
            throw "Exporting presets is not supported";
        fs::path file = stc::cwmm::ram_collection(name.string());

        std::vector<wmmb> newstruct = parser(file);

        if (fs::exists(directory)) {
            std::vector<wmmb> oldstruct = parser(oldFile);

            optimizations(newstruct, oldstruct);
            clearing(oldstruct, directory);
            compiller(newstruct, directory);

            fs::remove(oldFile);
        }
        else {
            fs::create_directories(directory);
            compiller(newstruct, directory);
        }
        collection_info(newstruct, oldFile, name.string());
    }
    catch (const std::string& error) {
        ERRORdialog* dialog = new ERRORdialog(error);
    }
    catch (const std::exception& e) {
        ERRORdialog* dialog = new ERRORdialog(std::string("Error: ") + e.what());
    }
}


Core::CollectionInfo::CollectionInfo (const std::filesystem::path& name) {
    wmml file(stc::cwmm::ram_collection(name.string()));
    std::vector<std::string> buffer;
    std::vector<wmml::variant> v(file.width());

    while (file.read(v)) {
        if (std::get<bool>(v[2]))
            ++mods;
        else {
            buffer.emplace_back(std::get<std::string>(v[0]));
            ++presets;
        }
    }

    allMods = mods;
    for (const auto& presetName : buffer) {
        wmml file(stc::cwmm::ram_preset(presetName));
        while(file.read(v)) {
            assert(std::get<bool>(v[2]));
            ++allMods;
        }
    }
}


void Core::exporter (const std::string& name, const bool monolith) {
    std::string exportString = EXPORT + name;
    std::string tempName = name + EXPANSION;
    try {
        if (std::filesystem::exists(exportString))
            std::filesystem::remove(exportString);
        ArchiveWriter archive(exportString);
        std::filesystem::path filename = stc::cwmm::ram_collection(name);
        std::vector<std::string> presets;
        std::vector<Core::wmmb> modlist = parser(filename, &presets);

        for (const auto& entry : modlist) {
            archive.write_in_archive(stc::cwmm::ram_mods(entry.name), stc::cwmm::ram_mods(entry.name));
            archive.write_in_archive((ARCHIVE + Core::CONFIG_GAME + "/" + std::to_string(entry.id) + "/" + entry.version + EXPANSION2),
                                     (ARCHIVE + Core::CONFIG_GAME + "/" + std::to_string(entry.id)));
        }

        if (monolith) {
            collection_info(modlist, tempName, name);
            archive.write_in_archive(tempName, stc::cwmm::ram_collection());
        }
        else {
            archive.write_in_archive(filename, stc::cwmm::ram_collection());
            for (const auto& entry : presets)
                archive.write_in_archive(stc::cwmm::ram_preset(entry), stc::cwmm::ram_preset());
        }
        std::filesystem::remove(tempName);
    }
    catch (const std::string& error) {
        std::filesystem::remove(exportString);
        std::filesystem::remove(tempName);

        ERRORdialog* dialog = new ERRORdialog(error);
    }
    catch (const std::exception& e) {
        ERRORdialog* dialog = new ERRORdialog(std::string("Error: ") + e.what());
    }
}


void Core::importer(const std::string& path) {
    if (path.empty())
        return;
    try {
        import target(path);
    }
    catch (const std::string& e) {
        ERRORdialog* dialog = new ERRORdialog(e);
    }
}
