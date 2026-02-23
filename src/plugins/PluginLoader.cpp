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
#include "PluginLoader.h"
#include "../methods.h"
#include "../core.h"
#include "PluginInterface.h"
#include <QApplication>
#include <QDir>
#include <QPluginLoader>


PluginLoader::PluginLoader () {
    Core::get().set_default("WMM_EXPANSION_DEBUG", "false");
    Core::get().set_default("WMM_ENABLE_PLUGINS", "true");
    if (Core::configs["WMM_ENABLE_PLUGINS"] == "true") {
        scan_directory();
        demons_init();
    }
}


PluginLoader::~PluginLoader () {
    for (auto* entry : list)
        delete entry;
}


PluginLoader& PluginLoader::get () {
    static PluginLoader singleton;
    return singleton;
}


void PluginLoader::scan_directory () {
    QDir dir(QApplication::applicationDirPath() + "/plugins/");
    if (dir.exists()) {
        QString pluginPath;
        QObject* pobj = nullptr;
        PluginInterface* pluginPtr = nullptr;
        for (QString& entry : dir.entryList(QStringList("*"), QDir::Dirs)) {
            if (entry == ".." || entry == ".")
                continue;
            QDir moduleDir = dir.path() + "/" + entry;
            for (QString& entry : moduleDir.entryList(QStringList("*"), QDir::Files))
                if (entry.endsWith(".so") || entry.endsWith(".dll"))
                    pluginPath = moduleDir.path() + "/" + entry;
            QPluginLoader loader(pluginPath);

            pobj = loader.instance();
            if (pobj) {
                pluginPtr = dynamic_cast<PluginInterface*>(pobj);
                if (pluginPtr) {
                    list.emplace_back(pluginPtr);
                    pluginPtr = nullptr;
                    pobj = nullptr;
                    exp_cerr(pluginPath.toStdString() + " has been started.");
                }
                else
                    stc::cerr(pluginPath.toStdString() + " " + loader.errorString().toStdString());
            }
            else
                stc::cerr(pluginPath.toStdString() + " " + loader.errorString().toStdString() +
                          " was not started");
        }
    }
}


void PluginLoader::demons_init () {
    for (auto* entry : list) {
        if (entry->is_demon()) {
            entry->main();
            exp_cerr(entry->name().toStdString() + " has been launched.");
        }
    }
}


void PluginLoader::exp_cerr (const std::string& value) {
    if (Core::configs["WMM_EXPANSION_DEBUG"] == "true")
        stc::cerr(value);
}


std::vector<PluginInterface*> PluginLoader::get_plugins_list () { return PluginLoader::get().list; }
