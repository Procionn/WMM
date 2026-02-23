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
#include "../plugins/PluginLoader.h"
#include "../plugins/PluginInterface.h"


setextensions::setextensions() {
    list = new QVBoxLayout;
    list->setAlignment(Qt::AlignTop);
    addScrollable(this, list);
    generate_buttons();
}

setextensions::~setextensions() {
    clear_list();
}


void setextensions::clear_list() {
    if (!expansionList.empty()) {
        for (auto* entry : expansionList)
            delete entry;
        expansionList.clear();
    }
}


void setextensions::generate_buttons() {
    clear_list();
    CLinkTumbler* last = nullptr;
    for (auto* plugin : PluginLoader::get_plugins_list()) {
        CLinkTumbler* button = new CLinkTumbler(plugin->name().toStdString(), last);
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [plugin]{plugin->main();});
        last = button;
        expansionList.emplace_back(button);
    }
}
