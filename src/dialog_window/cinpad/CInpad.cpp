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

bool CInpad::nameTest (std::vector<Cbox>& existsElements, std::string str) {
    //  проверяет, не существует ли объекта
    for (int i = existsElements.size(); i != 0; --i)
        if (str == existsElements[i].path)
            return false;
    return true;
}

void CInpad::reader () {
    std::string stringDir1 = stc::cwmm::ram_preset();
    std::string stringDir2 = stc::cwmm::ram_mods();
    std::string targetFiledDirectory;
    if ((*target)->type)
        targetFiledDirectory = stc::cwmm::ram_preset((*target)->name);
    else
        targetFiledDirectory = stc::cwmm::ram_collection((*target)->name);

    wmml file(targetFiledDirectory);
    std::vector<wmml::variant> v(file.width());
    std::vector<Cbox> existsElements;
    existsElements.reserve(file.height());

    for (int arraySize = 0; file.read(v); arraySize++) {
        if (std::get<bool>(v[2]))
            existsElements[arraySize].path = stc::cwmm::ram_mods(std::get<std::string>(v[0]));
        else
            existsElements[arraySize].path = stc::cwmm::ram_preset(std::get<std::string>(v[0]));
    }
    fsScaner(stringDir1, true, existsElements);
    fsScaner(stringDir2, false, existsElements);
    vector = true;
}


void CInpad::fsScaner(const std::filesystem::path& directory, const bool& type,
                      std::vector<Cbox>& existsElements) {
    if (std::filesystem::exists(directory)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory}) {
            std::string newButton = objects.path().string();
            stc::string::replace(newButton, '\\', '/');
            if (nameTest(existsElements, newButton)) {
                size_t part = newButton.find_last_of('/');
                newButton = newButton.substr(part + 1);
                CInpadButton* button;
                if (type) {
                    newButton = newButton.substr(0, newButton.size() - MAIN_PART);
                    button = new CInpadButton(newButton, true, count_type);
                }
                else
                    button = new CInpadButton(newButton, false, count_type);
                newObjectList->add(button);
                vlist.emplace_back(button);
            }
        }
    }
}


void CInpad::application(std::string& targetName, bool targetType) {
    std::string name;
    if (targetType) name = stc::cwmm::ram_preset(targetName);
    else            name = stc::cwmm::ram_collection(targetName);
    wmml file(name);
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
    this->reject();
}
