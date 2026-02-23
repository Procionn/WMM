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
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <vector>
#include <string>

class PluginInterface;
class PluginLoader
{
    std::vector<PluginInterface*> list;

    PluginLoader();
    ~PluginLoader();
    void scan_directory();
    void demons_init();
    void exp_cerr(const std::string& value);

public:
    static PluginLoader& get();
    static std::vector<PluginInterface*> get_plugins_list();
};

#endif // PLUGINLOADER_H
