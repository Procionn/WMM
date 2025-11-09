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
#include "setlang.h"
#include "../core.h"
#include "../CONSTANTS.h"
#include "../methods.h"
#include "settings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <filesystem>

setlang::setlang () {
    QVBoxLayout* list = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;
    QLabel* lang = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CHOOSE_LANG"]));
    QLabel* community = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CUSTOM_LANG"]).replace("\\n", "\n"));
    QPushButton* button = new QPushButton(QString::fromStdString(stc::string::get_name(Core::CONFIG_LANGUAGES)));
    addScrollable(this, list);

    list->setAlignment(Qt::AlignTop);
    hbox->setAlignment(Qt::AlignLeft);
    community->setWordWrap(true);

    list->addLayout(hbox);
    hbox->addWidget(lang);
    hbox->addWidget(button);
    list->addWidget(community);

    connect(CSettings::get(), &CSettings::save, [this]{
        if (target) {
            CConfigs::CONFIG_LANGUAGES = LANG + target->name + EXPANSION3;
            Core::get().overwriting_config_data();
            Core::get().update_lang();
            delete target;
            target = nullptr;
            FatalError* dialog = new FatalError(Core::lang["LANG_LABEL_NEW_LANG"]);
        }
    });

    connect(button, &QPushButton::clicked, [this, button]{chooseLang(button);});
}


void setlang::chooseLang (QPushButton* parent) {
    CFastDialog* chooser = new CFastDialog;
    QVBoxLayout* list = new QVBoxLayout;
    addScrollable(chooser->list, list);

    chooser->show();
    list->setAlignment(Qt::AlignTop);

    CLinkTumbler* lastBTN = nullptr;
    std::string lang;
    for (const auto& entry : std::filesystem::directory_iterator(LANG)) {
        lang = entry.path().generic_string().substr(0, entry.path().generic_string().size() - INI_MAIN_PART);
        size_t part = lang.find_last_of('/');
        lang = lang.substr(part + 1);
        CLinkTumbler* button = new CLinkTumbler(lang, lastBTN);
        lastBTN = button;
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [this, button]{
            target = button;
        });
        connect(chooser->apply, &QPushButton::clicked, [parent, this, chooser, list]{
            parent->setText(QString::fromStdString(target->name));
            list->removeWidget(target);
            target->setParent(nullptr);
            delete chooser;
        });
    }
}
