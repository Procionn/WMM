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
#include <wmml.h>
#include <QLineEdit>
#include <regex>

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
    QLineEdit* searchTab = new QLineEdit;
    horBox->addWidget(label);
    horBox->addWidget(menuBar);
    horBox->addWidget(searchTab);
    menuBar->setMinimumWidth(80);
    menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    searchTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    menu = new QMenu("v");
    menuBar->addMenu(menu);

    presets = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_PRESETS"]));
    menu->addAction(presets);
    connect(presets, &QAction::triggered, [this]{
        menu->setTitle(QString::fromStdString(Core::lang["LANG_BUTTON_PRESETS"]));
        targetType = true;
        distributor();
    });

    QAction* mods = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_MODS"]));
    menu->addAction(mods);
    connect(mods, &QAction::triggered, [this]{
        menu->setTitle(QString::fromStdString(Core::lang["LANG_BUTTON_MODS"]));
        targetType = false;
        distributor();
    });
    connect(searchTab, &QLineEdit::textEdited, this, &CInpad::search_slot);

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

void CInpad::render() {
    search("", false);
}

void CInpad::search (const QString& string, const bool flag) {
    static std::string ref;
    if (flag) ref = string.toStdString();
    for (auto* target : vlist) {
        if (target->type == targetType && std::regex_search(
                target->get_name(), std::regex(ref, std::regex_constants::icase)))
            target->show();
        else target->hide();
    }
    coloring();
}

void CInpad::search_slot (const QString& string) {
    search(string, true);
}

void CInpad::coloring () {
    CInpadButton* last = nullptr;
    for (auto* entry : vlist) {
        if (entry->isVisible()) {
            if (!last) {
                entry->set_style(true);
                last = entry;
            }
            else {
                entry->set_style(!(last->get_style()));
                last = entry;
            }
        }
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
    std::sort(vlist.begin(), vlist.end(), [](CInpadButton* a, CInpadButton* b) {
        return a->get_name() < b->get_name();
    });
    for (auto* button : vlist)
        newObjectList->add(button);
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
                vlist.emplace_back(button);
            }
        }
    }
}


void CInpad::mods_scaner(const std::vector<std::string>& existsElements) {
    for (const auto& entry : ModManager::get().all_mods_list()) {
        std::string buttonName = ModManager::get().mod_data_converter(entry->modId);
        if (not_exists(existsElements, buttonName)) {
            CInpadButton* button = new CInpadButton(buttonName, false, count_type);
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
                v = {name, "", false, (uint64_t)(0), true};
                file.write(v);
            }
            else {
                uint64_t id = ModManager::get().mod_data_converter(name);
                v = {name, ModManager::get().mod_recommended_version(id), true, id, true};
                file.write(v);
            }
        }
    }
    reset();
    reject();
}
