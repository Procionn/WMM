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
#include "ModStorageObject.h"

#include "../../patterns/CSwitchButton.h"
#include "../../ModManager.h"
#include "../../methods.h"
#include "ModVersionList.h"
#include <QLabel>
#include <QVBoxLayout>

ModObject::ModObject (const Mod* base) : data(base), versions(nullptr), CBaseSmartObject(false) {
    QVBoxLayout* baseLay = new QVBoxLayout(this);
    baseLay->addWidget(main = new QWidget);

    QHBoxLayout* mainLay = new QHBoxLayout(main);
    CSwitchButton* openVersions = new CSwitchButton("v", "^", false);
    QLabel* name = new QLabel(QString::fromStdString(
        ModManager::get().mod_data_converter(base->modId)));
    QLabel* id = new QLabel(QString::number(base->modId));
    openVersions->setFixedWidth(15);
    mainLay->addWidget(openVersions);
    mainLay->addWidget(name);
    mainLay->addWidget(id);

    connect(openVersions, &CSwitchButton::toggled, [this, baseLay]{
        if (versions) {
            versions->show();
        }
        else {
            baseLay->addWidget(versions = new QWidget);
            versions->setStyleSheet("background-color: #363638");
            list = new ModVersionList();
            QVBoxLayout* l = new QVBoxLayout(versions);
            l->addWidget(list);
            for (const auto& entry : ModManager::get().all_versions_list(data->modId)) {
                list->add(new ModVersionObject(entry.data(), this));
            }
            connect(list, &ModVersionList::allOn, this, &ModObject::none_triggered_on);
            connect(list, &ModVersionList::noAllOn, this, &ModObject::none_triggered_off);
            connect(list, &ModVersionList::allOn, [this]{emit fromChildSwitched(this);});
            connect(this, &ModObject::childs_set_on, list, &ModVersionList::set_on);
            connect(this, &ModObject::childs_set_off, list, &ModVersionList::set_off);
        }
    });
    connect(openVersions, &CSwitchButton::untoggled, [this] {
        versions->hide();
    });
    setFrameStyle(QFrame::Box | QFrame::Raised);
    if (count_type)
        setStyleSheet(QString::fromStdString(dataString + untoggledColor1));
    else
        setStyleSheet(QString::fromStdString(dataString + untoggledColor2));
}



void ModObject::DELETE() {
    ModManager* module = &ModManager::get();
    if (module->exists(data->modId, (data->recommended_version())))
        module->remove(data->modId);
    emit remove(this);
}

void ModObject::INFO() {
    stc::net::openURL(stc::cwmm::modsURL(std::to_string(data->modId)));
}

void ModObject::turnOff () {
    none_triggered_off();
    emit childs_set_off();
}

void ModObject::turnOn () {
    none_triggered_on();
    emit childs_set_on();
}

void ModObject::child_turnOff () {
    if (list)
        list->set_off();
}

void ModObject::none_triggered_on () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(dataString + toggledColor1));
        else
            setStyleSheet(QString::fromStdString(dataString + toggledColor2));
    }
}

void ModObject::none_triggered_off () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(dataString + untoggledColor1));
        else
            setStyleSheet(QString::fromStdString(dataString + untoggledColor2));
    }
}

std::string ModObject::get_name () {
    return ModManager::get().mod_data_converter(data->modId);
}
