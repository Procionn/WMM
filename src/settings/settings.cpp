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
#include "settings.h"
#include "../CONSTANTS.h"
#include "../patterns/CScrollWindow.h"
#include "../core.h"
#include "../ModManager.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>


CSettings::CSettings () {
    setMinimumHeight(200);
    setMinimumWidth(400);
    resize(1000, 600);
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    QHBoxLayout* dialogButtonBox = new QHBoxLayout();
    QPushButton* accept = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_ACCEPT"]));
    QPushButton* cansel = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_CANSEL"]));
    QWidget* separator = new QWidget;
    QSplitter* splitter = new QSplitter;
    QFrame* line = new QFrame();
    sobjects = new SObjects;
    settings_modules_list = new SList;
    
    settings_modules_list->resize(500, 10);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(cansel, &QPushButton::clicked, this, [=]{this->reject();});
    connect(accept, &QPushButton::clicked, this, &CSettings::save);
    connect(sobjects->sorce,        &CLinkTumbler::toggled, settings_modules_list, &SList::sorce);
    connect(sobjects->lang,         &CLinkTumbler::toggled, settings_modules_list, &SList::lang);
    connect(sobjects->support,      &CLinkTumbler::toggled, settings_modules_list, &SList::support);
    connect(sobjects->collections,  &CLinkTumbler::toggled, settings_modules_list, &SList::collection);
    sobjects->sorce->isTarget(true);
    
    dialogLayout->addLayout(dialogButtonBox);
    dialogLayout->addWidget(line);
    dialogButtonBox->addWidget(accept);
    dialogButtonBox->addWidget(separator);
    dialogButtonBox->addWidget(cansel);
    dialogLayout->addWidget(splitter);
    splitter->addWidget(sobjects);
    splitter->addWidget(settings_modules_list);
}

void CSettings::save () {
    if (settings_modules_list->settings_source->target) {
        Core::CONFIG_GAME = settings_modules_list->settings_source->target->name;
        Core::get().overwriting_config_data();
        settings_modules_list->settings_source->target = nullptr;
        Core::get().update_data_from_file();
        ModManager::get().update();
    }
    if (!settings_modules_list->settings_source->buffer.isEmpty()) {
        Core::get().save_game_path(settings_modules_list->settings_source->buffer.toStdString());
        settings_modules_list->settings_source->buffer.clear();
    }
    if (settings_modules_list->settings_lang->target) {
        CConfigs::CONFIG_LANGUAGES = LANG + settings_modules_list->settings_lang->target->name + EXPANSION3;
        Core::get().overwriting_config_data();
        Core::get().update_lang();
        FatalError* dialog = new FatalError(Core::lang["LANG_LABEL_NEW_LANG"]);
        settings_modules_list->settings_lang->target = nullptr;
    }
}





SObjects::SObjects () {
    list = new QVBoxLayout;
    addScrollable(this, list);
    sorce = new CLinkTumbler(Core::lang["LANG_BUTTON_SORCE"]);
    lang = new CLinkTumbler(Core::lang["LANG_BUTTON_LANG"], sorce);
    support = new CLinkTumbler(Core::lang["LANG_BUTTON_SUPPORT"], lang);
    extensions = new CLinkTumbler(Core::lang["LANG_BUTTON_EXTENSIONS"], support);
    collections = new CLinkTumbler(Core::lang["LANG_BUTTON_IMPORT/EXPORT"], extensions);
    
    list->setAlignment(Qt::AlignTop);
    sorce->SetLeftAlignment(true);
    lang->SetLeftAlignment(true);
    support->SetLeftAlignment(true);
    extensions->SetLeftAlignment(true);
    collections->SetLeftAlignment(true);
    ///////////////////////////////////
    // support->hide();
    extensions->hide();
    ///////////////////////////////////
    sorce->setMinimumHeight(35);
    lang->setMinimumHeight(35);
    support->setMinimumHeight(35);
    extensions->setMinimumHeight(35);
    collections->setMinimumHeight(35);
    
    list->addWidget(sorce);
    list->addWidget(lang);
    list->addWidget(extensions);
    list->addWidget(collections);
    list->addWidget(support);
}





SList::SList () {
    QVBoxLayout* list   = new QVBoxLayout(this);
    settings_source     = new setsource;
    settings_lang       = new setlang;
    settings_support    = new setsupport;
    settings_extension  = new setextensions;
    settings_collections= new collections;
    list->addWidget(settings_source);
    list->addWidget(settings_lang);
    list->addWidget(settings_support);
    list->addWidget(settings_extension);
    list->addWidget(settings_collections);
}

void SList::sorce () {
    settings_source->show();
    settings_lang->hide();
    settings_support->hide();
    settings_extension->hide();
    settings_collections->hide();
}

void SList::lang () {
    settings_source->hide();
    settings_lang->show();
    settings_support->hide();
    settings_extension->hide();
    settings_collections->hide();
}

void SList::support () {
    settings_source->hide();
    settings_lang->hide();
    settings_support->show();
    settings_extension->hide();
    settings_collections->hide();
}

void SList::extensions () {
    settings_source->hide();
    settings_lang->hide();
    settings_support->hide();
    settings_extension->show();
    settings_collections->hide();
}

void SList::collection () {
    settings_source->hide();
    settings_lang->hide();
    settings_support->hide();
    settings_extension->hide();
    settings_collections->show();
}
