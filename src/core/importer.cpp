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
#include "../importer.h"

#include "../ModManager.h"
#include "../patterns/ERRORdialog.h"
#include "../patterns/RenameDialog.h"
#include "../patterns/WaitingWindow.h"
#include "../methods.h"
#include "../core.h"
#include "../CONSTANTS.h"
#include <hpp-archive.h>


import::import (const std::string& path) : archivePath(path) {
    collectionPath = stc::cwmm::ram_collection();
    presetPath = stc::cwmm::ram_preset();
    unarchivate_main_objects();
    Wait(
    if (presetsRenamed)
        renaming_fix();
    mods_import(mods_import_list());
    );
    ModManager::get().flush();
}


void import::unarchivate_main_objects () {
    bool error = true;
    ArchiveReader archive(archivePath);

    std::string archivedFilePath;
    for (const auto* entry : archive) {
#ifdef __linux__
        archivedFilePath = archive.get_target_filename();
#elif WIN64
        archivedFilePath = (std::filesystem::path(archive.get_target_filename()).string());
#endif

        // compare is used here instead of starts_with, because the program is tailored to cpp17.
        // In the future, support for the 20 format will be added and the functions will be rewritten using new tools.

        if (archivedFilePath.compare(0, collectionPath.size(), collectionPath) == 0) {
        // if (archivedFilePath.starts_with(collectionPath)) {
            if (std::filesystem::exists(archivedFilePath)) {
                std::string dir, expansion;
                renameMark:
                    mainCollectionFile = file_renaming(stc::string::get_name(archivedFilePath, dir, expansion),
                                                       Core::lang["LANG_LABEL_RENAME_COLLECTIONS"]);
                    if (mainCollectionFile.empty())
                        throw Core::lang["LANG_LABEL_EXPORT_INTERRUPTED"];
                    archivedFilePath = dir + mainCollectionFile + expansion;
                    if (std::filesystem::exists(archivedFilePath)) {
                        ERRORdialog* error = new ERRORdialog(Core::lang["LANG_LABEL_R34"]);
                        error->exec();
                        goto renameMark;
                    }
            }
            else {
                mainCollectionFile = stc::string::get_name(archivedFilePath);
            }
            archive.write_on_disk(archivedFilePath);
            error = false;
        }
        else if (archivedFilePath.compare(0, presetPath.size(), presetPath) == 0) {
        // else if (archivedFilePath.starts_with(presetPath)) {
            if (std::filesystem::exists(archivedFilePath)) {
                std::string dir, expansion;
                std::string oldPresetName = stc::string::get_name(archivedFilePath, dir, expansion);
                std::string newPresetName = mainCollectionFile + " - " + oldPresetName;
                archivedFilePath = dir + newPresetName + expansion;
                presetsRenamed = true;
                renamedList[oldPresetName] = newPresetName;
            }
            else {
                std::string oldPresetName = stc::string::get_name(archivedFilePath);
                renamedList[oldPresetName] = oldPresetName;
            }
            archive.write_on_disk(archivedFilePath);
        }
    }
    if (error) {
        ERRORdialog* dialog = new ERRORdialog("The main file was not found. The collection may be corrupted.");
        return;
    }
}


void import::renaming_fix () {
    //////////////////////////////////////////////////////////////////////////////////////////
    // This function will only exist here until the wmml library is modified
    // to fix a bug that makes it impossible to edit recorded lines.
    std::string filePath = stc::cwmm::ram_collection(mainCollectionFile);
    std::vector<std::vector<wmml::variant>> list;
    {
        wmml oldFile(filePath);
        list.reserve(oldFile.height());

        for (std::vector<wmml::variant> v(GRID_WIDTH); oldFile.read(v);)
            list.emplace_back(v);
    }
    std::filesystem::remove(filePath);
    wmml newFile(filePath, GRID_WIDTH);

    for (auto& entry : list) {
        if (!std::get<bool>(entry[2]))
            entry[0] = renamedList[std::get<std::string>(entry[0])];
        newFile.write(entry);
    }
}


void* import::mods_import_list () {
    std::vector<Core::wmmb> modlist = Core::get().parser(stc::cwmm::ram_collection(mainCollectionFile), nullptr, false);
    std::vector<Core::wmmb>* importList = new std::vector<Core::wmmb>;
    importList->reserve(modlist.size());

    for(auto& entry : modlist)
        if (!ModManager::get().exists(entry.id, entry.version))
            importList->emplace_back(entry);
    return static_cast<void*>(importList);
}


void import::mods_import (void* vector) {
    std::vector<Core::wmmb>* importList = static_cast<std::vector<Core::wmmb>*>(vector);

    std::string modsDirectory = stc::cwmm::ram_mods() + "/";
    std::string archiveDirectory = ARCHIVE + Core::CONFIG_GAME + "/";

    ArchiveReader archive(archivePath);
    std::string targetFilename;
    for (const auto* entry : archive) {
#ifdef __linux__
        targetFilename = archive.get_target_filename();
#elif WIN64
        targetFilename = (std::filesystem::path(archive.get_target_filename()).string());
#endif
        if (targetFilename.compare(0, modsDirectory.size(), modsDirectory) == 0) {
            std::string hash;
            for (const auto& importingMod : *importList) {
                hash = std::to_string(importingMod.id) + "/" + importingMod.version;
                if (targetFilename.compare(modsDirectory.size(), hash.size(), hash) == 0) {
                    archive.write_on_disk();
                    ModManager::get().add(importingMod.id, std::move(importingMod.version),
                                          std::move(importingMod.name));
                    break;
                }
            }
        }
        else if (targetFilename.compare(0, archiveDirectory.size(), archiveDirectory) == 0)
            if (!std::filesystem::exists(targetFilename))
                archive.write_on_disk();
    }

    delete importList;
}
