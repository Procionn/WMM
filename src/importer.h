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
#ifndef IMPORTER_H
#define IMPORTER_H

#include <string>
#include <map>
#include <vector>


class import
{
    const std::string archivePath;
    std::string mainCollectionFile;
    std::string collectionPath;
    std::string presetPath;
    std::map<std::string, std::string> renamedList;
    bool presetsRenamed = false;
public:
    import(const std::string& path);
private:
    void unarchivate_main_objects();
    void renaming_fix();
    void* mods_import_list();
    void mods_import(void*);
};


#endif // IMPORTER_H
