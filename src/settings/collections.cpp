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
#include "collections.h"

#include <QLabel>
#include <QGridLayout>
#include <QDesktopServices>
#include <QFileDialog>
#include "../patterns/CScrollWindow.h"
#include "../patterns/CSwitchButton.h"
#include "../core.h"
#include "../methods.h"
#include "../CONSTANTS.h"


collections::collections () {
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QVBoxLayout* left_list = new QVBoxLayout;
    collections_list = new QVBoxLayout;
    right_list = new QVBoxLayout;
    QWidget* pad = new QWidget;
    QFrame* line = new QFrame;
    QLabel* collections_label = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_COLLECTIONS"]));
    QPushButton* importButton = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_IMPORT"]));
    QPushButton* exportButton = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_EXPORT"]));
    settingsBox = new SettingsBox;

    hbox->setAlignment(Qt::AlignCenter);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    collections_list->setAlignment(Qt::AlignTop);
    right_list->setAlignment(Qt::AlignTop);
    addScrollable(pad, collections_list);

    hbox->addLayout(left_list);
    hbox->addWidget(line);
    hbox->addLayout(right_list);

    left_list->addWidget(collections_label);
    left_list->addWidget(pad);
    left_list->addWidget(importButton);

    update_list();
    update_collection_info(nullptr);
    right_list->addWidget(settingsBox);
    right_list->addWidget(exportButton);
    connect(importButton, &QPushButton::clicked, [&]{importing();});
    connect(exportButton, &QPushButton::clicked, [&]{exporting();});
}



void collections::update_list () {
    auto directory = stc::cwmm::ram_collection();
    if (!std::filesystem::exists(directory))
        return;
    for (QLayoutItem* item; (item = collections_list->takeAt(0)) != nullptr;) {
        delete item->widget();
        delete item;
    }
    CLinkTumbler* last = nullptr;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        CLinkTumbler* button = new CLinkTumbler(stc::string::get_name(entry.path().string()), last);
        connect(button, &CLinkTumbler::toggled, this, &collections::update_collection_info);
        button->SetLeftAlignment(true);
        button->setMinimumHeight(30);
        collections_list->addWidget(button);
        last = button;
    }
}


void collections::update_collection_info (CToggledButton* target) {
    static QLabel* mods;
    static QLabel* presets;
    static QLabel* allMods;
    if (!mods) {
        QFrame* main = new QFrame;
        QGridLayout* list = new QGridLayout(main);
        list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_MODS"])));
        list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_PRESETS"])));
        list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_ALL_MODS"])));
        mods = new QLabel;
        presets = new QLabel;
        allMods = new QLabel;

        main->setFrameShape(QFrame::WinPanel);
        main->setFrameShadow(QFrame::Raised);
        main->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        list->addWidget(mods,    0, 1);
        list->addWidget(presets, 1, 1);
        list->addWidget(allMods, 2, 1);
        right_list->addWidget(main);
    }

    if (target) {
        Core::CollectionInfo* info = new Core::CollectionInfo(target->name);
        mods->setText(QString::number(info->mods));
        presets->setText(QString::number(info->presets));
        allMods->setText(QString::number(info->allMods));
        this->target = target;
    }
    else {
        mods->setText(QString::number(0));
        presets->setText(QString::number(0));
        allMods->setText(QString::number(0));
    }
}


void collections::exporting () {
    if (!std::filesystem::exists(EXPORT))
         std::filesystem::create_directory(EXPORT);
    if (!settingsBox->online)
        Core::get().exporter(target->name, settingsBox->monolith);
    else
        stc::cerr("online export may be added later");
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(EXPORT))))
        throw ("Error when opening a directory");
}


void collections::importing () {
    QString buffer = QFileDialog::getOpenFileName(
        nullptr,
        QString::fromStdString(Core::lang["LANG_LABEL_CHOOSE_COLLECTION"]),
        "",
        QString::fromStdString(Core::lang["LANG_LABEL_ALL_FILE"] + " (*.tar.zst)")
    );
    Core::get().importer(buffer.toStdString());
    update_list();
    emit import_completed();
}





SettingsBox::SettingsBox () {
    QGridLayout* list = new QGridLayout(this);
    CSwitchButton* monolithButton = new CSwitchButton();
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_MONOLITH"])));
    list->addWidget(monolithButton, 0, 1);
    connect(monolithButton, &CSwitchButton::toggled, [&]{monolith = true;});
    connect(monolithButton, &CSwitchButton::untoggled, [&]{monolith = false;});

#if 0
    /*
     *  Online installation can be added later.
     *  For now, let the billet remain in the code.
    */
    CSwitchButton* onlineButton = new CSwitchButton();
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_ONLINE"])));
    list->addWidget(onlineButton, 1, 1);
    connect(onlineButton, &CSwitchButton::toggled, [&]{online = true;});
    connect(onlineButton, &CSwitchButton::untoggled, [&]{online = false;});
#endif

    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Sunken);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    list->setAlignment(Qt::AlignTop);
}
