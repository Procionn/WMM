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
#include "../../ModManager.h"
#include "../../methods.h"


ModStorageList::ModStorageList() {
    for (const Mod& mod : ModManager::get().all_mods_list()) {
        ModObject* modObject;
        add(modObject = new ModObject(&mod));
        connect(this, &ModStorageList::reseting, modObject, &ModObject::child_turnOff);
        connect(modObject, &ModObject::fromChildSwitched, this, &ModStorageList::last_target_update);
    }
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
