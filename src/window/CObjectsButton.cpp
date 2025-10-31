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
#include "CObjectsButton.h"

#include "../core.h"
#include "../methods.h"
#include "../CONSTANTS.h"

CObjectsButton::CObjectsButton(const std::string& name, CObjectsButton* linked) : CLinkTumbler(name){
    if (linked != nullptr) {
        link = linked;
        connect(this, &CObjectsButton::clicked, linked, &CObjectsButton::reset);
        connect(linked, &CObjectsButton::clicked, this, &CObjectsButton::reset);
        while (linked->link != nullptr) {
            linked = linked->link;
            connect(this, &CObjectsButton::clicked, linked, &CObjectsButton::reset);
            connect(linked, &CObjectsButton::clicked, this, &CObjectsButton::reset);
        }
    }
    setMinimumHeight(35);
    SetLeftAlignment(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CObjectsButton::context);
}


void CObjectsButton::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &CObjectsButton::DELETE);
    contextMenu->exec(this->mapToGlobal(pos));
}

void CObjectsButton::DELETE() {
    if (type) std::filesystem::remove(stc::cwmm::ram_preset(name));
    else {
        std::filesystem::remove(stc::cwmm::ram_collection(name));
        auto path = std::filesystem::path(COLLECTIONS + Core::CONFIG_GAME) / name;
        if (std::filesystem::exists(path))
            stc::fs::remove_all(path);
    }
    emit remove(this);
}
