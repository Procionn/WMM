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
#include "setextensions.h"
#include <QPluginLoader>
#include <QString>
#include <QDir>
#include <QApplication>
#include <QVBoxLayout>
#include "../methods.h"
#include "../patterns/CScrollWindow.h"
#include "../patterns/CLinkTumbler.h"


setextensions::setextensions() {
    scan_dir();
    list = new QVBoxLayout;
    list->setAlignment(Qt::AlignTop);
    addScrollable(this, list);
    CLinkTumbler* last = nullptr;
    for (auto* plugin : expansionList) {
        CLinkTumbler* button = new CLinkTumbler(plugin->name().toStdString(), last);
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [plugin]{plugin->plugin_main();});
        last = button;
    }
}

setextensions::~setextensions() {
    clear_list();
}


void setextensions::scan_dir () {
    clear_list();
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
            if(pobj) {
                pluginPtr = dynamic_cast<PluginInterface*>(pobj);
                if(pluginPtr) {
                    expansionList.emplace_back(pluginPtr);
                    pluginPtr = nullptr;
                    pobj = nullptr;
                }
                else
                    stc::cerr(pluginPath.toStdString() + " " + loader.errorString().toStdString());
            }
            else
                stc::cerr(pluginPath.toStdString() + " " + loader.errorString().toStdString() + " was not started");
        }
    }
}


void setextensions::clear_list() {
    if (!expansionList.empty()) {
        for (auto* entry : expansionList)
            delete entry;
        expansionList.clear();
    }

    for (QLayoutItem* item; (item = list->takeAt(0)) != nullptr;) {
        delete item->widget();
        delete item;
    }
}
