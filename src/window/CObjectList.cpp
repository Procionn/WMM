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
#include "mainWidgets.h"

#include "../patterns/ERRORdialog.h"
#include "../core.h"
#include <QTimer>

CObjectList::CObjectList () {
    setMaximumWidth(1000);
    setMinimumWidth(230);
    QVBoxLayout* privateObjectList = new QVBoxLayout;
    setLayout(privateObjectList);
    QHBoxLayout* objectButtonBox = new QHBoxLayout();
    privateObjectList->addLayout(objectButtonBox);
    CLinkTumbler* Collection = new CLinkTumbler(Core::lang["LANG_BUTTON_COLLECTIONS"]);
    CLinkTumbler* Preset     = new CLinkTumbler(Core::lang["LANG_BUTTON_PRESETS"], Collection);
    objectButtonBox->addWidget(Collection);
    objectButtonBox->addWidget(Preset);
    Collection->isTarget(true);
    Collection->setMinimumHeight(35);
    Preset->setMinimumHeight(35);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    privateObjectList->addWidget(line);

    objectList = new QVBoxLayout();
    addScrollable(privateObjectList, objectList);
    privateObjectList->setAlignment(Qt::AlignTop);
    objectList->setAlignment(Qt::AlignTop);

    connect(Collection, &QPushButton::clicked, this, [=]{
        TypeTarget = false;
        render();
    });

    connect(Preset,     &QPushButton::clicked, this, [=]{
        TypeTarget = true;
        render();
    });
    updateList();
    render();
}

void CObjectList::newObject (CNewObjectDialog* dialog) {
    QString tab = dialog->nameTab->text();
    CreteObject(tab.toStdString());
    dialog->reject();
}


void CObjectList::CreteObject (std::string name) {
    std::filesystem::path path;
    if (TypeTarget) path = stc::cwmm::ram_preset(name);
    else            path = stc::cwmm::ram_collection(name);
    if (!std::filesystem::exists(path)) {
        stc::wmm::new_object(path);
        updateList();
        render();
    }
    else ERRORdialog* errorR34 = new ERRORdialog(Core::lang["LANG_LABEL_R34"]);
}



void CObjectList::scan_directory (const std::filesystem::path& directory, const bool type, CObjectsButton*& lastTumbler) {
    if (std::filesystem::exists(directory)) {
        for (auto const& object : std::filesystem::directory_iterator(directory)) {
            std::string newButton = stc::string::get_name(object.path().string());
            CObjectsButton* button = new CObjectsButton(newButton, lastTumbler);
            button->type = type;
            objectList->addWidget(button);
            connect(button, &QPushButton::clicked, this, [=]{
                emit objectChoosed(button, TypeTarget);
                targetName = newButton;
            });
            connect(button, &CObjectsButton::remove, this, [=]{
                updateList();
                render();
                emit remove(button);
            });
            button->hide();
            list.emplace_back(button);
            lastTumbler = button;
        }
    }
}


void CObjectList::updateList () {
    CObjectsButton* lastTumbler = nullptr;
    for (const auto& entry : list)
        delete entry;
    list.clear();

    scan_directory(stc::cwmm::ram_preset(),     true, lastTumbler);
    scan_directory(stc::cwmm::ram_collection(), false, lastTumbler);
}

void CObjectList::render() {
    if (TypeTarget) {
        for (CObjectsButton* target : list)
            if  (target->type == true) target->show();
            else target->hide();
    }
    else {
        for (CObjectsButton* target : list)
            if  (target->type == false) target->show();
            else target->hide();
    }
}

