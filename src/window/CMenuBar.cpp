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
#include "CMenuBar.h"

#include "../window.h"
#include "../patterns/WaitingWindow.h"

CMenuBar::CMenuBar(Window* parent) {
    this->parent = parent;

    QAction* settings       = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_SETTINGS"]), this);
    QAction* object         = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_NEW_OBJECT"]), this);
    QAction* assembling     = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_ASSEMBLING"]), this);
    QAction* reassembly     = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_REASSEMBLY"]), this);
    QAction* linking        = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_LINKING"]), this);
    QAction* content        = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_ADD_COMPONENTS"]), this);

    addAction(settings);
    addAction(object);
    addAction(content);
    addAction(assembling);
    addAction(reassembly);
    addAction(linking);

    connect(content,    &QAction::triggered, [=]{emit content_click();});
    connect(settings,   &QAction::triggered, [=]{emit settings_click();});
    connect(object,     &QAction::triggered, [=]{emit object_click();});
    connect(assembling, &QAction::triggered, this, &CMenuBar::assembling_clicked);
    connect(reassembly, &QAction::triggered, this, &CMenuBar::reassembly_clicked);
    connect(linking,    &QAction::triggered, this, &CMenuBar::linking_clicked);
}



void CMenuBar::assembling_clicked () {
    if (parent->target != nullptr) {
        if (parent->target->type)
            ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
        else {
            Wait(
                Core::get().collector(parent->target->name, parent->ObjectList->TypeTarget);
            );
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R31"]);
}



void CMenuBar::linking_clicked () {
    if (parent->target != nullptr) {
        if (parent->target->type)
            ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
        else {
            if (std::filesystem::exists(stc::cwmm::backup_path())) {
                Wait(
                    Core::get().symlink_deliting();
                    Core::get().symlink_creating(parent->target->name);
                );
            }
            else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R38"]);
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R31"]);
}



void CMenuBar::reassembly_clicked () {
    if (parent->target != nullptr && !parent->target->type) {
        if (Core::CONFIG_GAME_PATH != "") {
            std::filesystem::remove_all(COLLECTIONS + Core::CONFIG_GAME + "/" + parent->target->name);
            Wait(
                Core::get().collector(parent->target->name, parent->ObjectList->TypeTarget);
            );
        }
    }
    else if (parent->target->type)
         ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R36"]);
}
