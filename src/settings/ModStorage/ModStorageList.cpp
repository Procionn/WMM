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
#include "ModStorageList.h"

#include <regex>
#include <QMenuBar>
#include <QAction>
#include "../../core.h"
#include "../../ModManager.h"
#include "../../methods.h"


ModStorageList::ModStorageList() {
    scaner();
}

void ModStorageList::update () {
    clear();
    scaner();
}

void ModStorageList::scaner () {
    ModObject* modObject;
    for (const Mod* mod : ModManager::get().all_mods_list()) {
        modObject = new ModObject(mod);
        add(modObject);
        connect(this, &ModStorageList::reseting, modObject, &ModObject::child_turnOff);
        connect(modObject, &ModObject::fromChildSwitched, this, &ModStorageList::last_target_update);
        connect(modObject, &ModObject::remove, this, &ModStorageList::delete_target);
    }
}



void ModStorageList::RMB (const QPoint&, ModObject*) {
    QMenu* contextMenu = new QMenu(this);
    QAction* action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &ModStorageList::deletion_signals);
    contextMenu->exec(QCursor::pos());
}

void ModStorageList::deletion_signals () {
    std::vector<ModObject*> newVector;
    newVector.reserve(childList.size());

    for (auto* target : childList) {
        if (target->is_target())
            newVector.emplace_back(target);
    }

    for (auto* target : newVector)
        target->DELETE();
}


void ModStorageList::delete_target(ModObject* target) {
    auto iterator = std::find(childList.begin(), childList.end(), target);
    if (iterator == childList.end())
        std::runtime_error(std::string("the object ") + target->get_name() + " was not found");
    childList.erase(iterator);
    delete target;
}


void ModStorageList::last_target_update (ModObject* new_target) {
    lastTarget = new_target;
}


void ModStorageList::search(const QString& ref) {
    for (auto* target : childList) {
        if (std::regex_search(target->get_name(), std::regex(ref.toStdString(), std::regex_constants::icase)))
            target->show();
        else target->hide();
    }
}
