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
#include "CInpad.h"

#include "../../ModManager.h"
#include "../../core.h"
#include <QMenuBar>

CInpad::CInpad(bool& type) {
    callType = &type;
    setMinimumHeight(400);
    setMinimumWidth(600);
    autodeleting = false;
    QVBoxLayout* vertBox = new QVBoxLayout(list);
    QHBoxLayout* horBox = new QHBoxLayout;
    vertBox->addLayout(horBox);
    QLabel* label = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_ADD"]));
    QMenuBar* menuBar = new QMenuBar;
    horBox->addWidget(label);
    horBox->addWidget(menuBar);

    menu = new QMenu("v");
    menuBar->addMenu(menu);

    presets = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_PRESETS"]));
    menu->addAction(presets);
    connect(presets, &QAction::triggered, this, [=]{
        menu->setTitle(QString::fromStdString(Core::lang["LANG_BUTTON_PRESETS"]));
        targetType = true;
        distributor();
    });

    QAction* mods = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_MODS"]));
    menu->addAction(mods);
    connect(mods, &QAction::triggered, this, [=]{
        menu->setTitle(QString::fromStdString(Core::lang["LANG_BUTTON_MODS"]));
        targetType = false;
        distributor();
    });

    newObjectList = new CInpadList;
    vertBox->addWidget(newObjectList);
}

void CInpad::reset() {
    newObjectList->clear();
    vector = false;
    vlist.clear();
    menu->setTitle("v");
    if (!(*callType)) presets->setVisible(true);
    else presets->setVisible(false);
}

void CInpad::distributor() {
    if (!vector)
        reader();
    render();
}

void CInpad::render () {
    if (targetType) {
        for (CInpadButton* target : vlist)
            if  (target->type == true)
                target->show();
            else target->hide();
    }
    else {
        for (CInpadButton* target : vlist)
            if  (target->type == false)
                target->show();
            else target->hide();
    }
}

bool CInpad::not_exists (const std::vector<std::string>& existsElements, const std::string& str) {
    for (const auto& entry : existsElements)
        if (str == entry)
            return false;
    return true;
}

void CInpad::reader () {
    std::string targetFiledDirectory;
    if ((*target)->type)
        targetFiledDirectory = stc::cwmm::ram_preset((*target)->name);
    else
        targetFiledDirectory = stc::cwmm::ram_collection((*target)->name);

    wmml file(targetFiledDirectory);
    std::vector<wmml::variant> v(file.width());
    std::vector<std::string> existsElements;
    existsElements.reserve(file.height());

    for (int arraySize = 0; file.read(v); arraySize++)
        if (std::get<bool>(v[2]))
            existsElements.emplace_back(std::get<std::string>(v[0]));
        else
            existsElements.emplace_back(stc::cwmm::ram_preset(std::get<std::string>(v[0])));
    presets_directory_scaner(existsElements);
    mods_scaner(existsElements);
    vector = true;
}


void CInpad::presets_directory_scaner(const std::vector<std::string>& existsElements) {
    std::filesystem::path directory = stc::cwmm::ram_preset();
    if (std::filesystem::exists(directory)) {
        for (auto const& objects : std::filesystem::directory_iterator(directory)) {
            std::string newButton = objects.path().string();
            stc::string::replace(newButton, '\\', '/');
            if (not_exists(existsElements, newButton)) {
                newButton = stc::string::get_name(newButton);
                CInpadButton* button = new CInpadButton(newButton, true, count_type);
                newObjectList->add(button);
                vlist.emplace_back(button);
            }
        }
    }
}


void CInpad::mods_scaner(const std::vector<std::string>& existsElements) {
    for (const auto& entry : ModManager::get().all_mods_list()) {
        std::string buttonName = ModManager::get().mod_data_converter(entry.modId);
        if (not_exists(existsElements, buttonName)) {
            CInpadButton* button = new CInpadButton(buttonName, false, count_type);
            newObjectList->add(button);
            vlist.emplace_back(button);
        }
    }
}


void CInpad::application(const std::string& targetName, const bool targetType) {
    std::string fileName;
    if (targetType) fileName = stc::cwmm::ram_preset(targetName);
    else            fileName = stc::cwmm::ram_collection(targetName);
    wmml file(fileName);
    std::vector<wmml::variant> v(file.width());
    for (CInpadButton* target : vlist) {
        if (target->is_target()) {
            std::string name = target->get_name();
            if  (target->type == true) {
                v = {name, "", false, (unsigned long)(0), true};
                file.write(v);
            }
            else {
                unsigned long id = ModManager::get().mod_data_converter(name);
                v = {name, ModManager::get().mod_recommended_version(id), true, id, true};
                file.write(v);
            }
        }
    }
    reset();
    reject();
}
