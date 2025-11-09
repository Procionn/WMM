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
#include "setsource.h"

#include <filesystem>
#include <QFileDialog>
#include <QLabel>
#include "../methods.h"
#include "../patterns/WaitingWindow.h"
#include "../ModManager.h"
#include "../patterns/CSwitchButton.h"
#include "settings.h"

setsource::setsource() {
    QHBoxLayout* list = new QHBoxLayout;
    QFrame* line = new QFrame();
    firstlist = new QVBoxLayout;
    lastlist = new QVBoxLayout;
    QWidget* firstwidget = new QWidget;
    QWidget* lastwidget = new QWidget;
    addScrollable(this, list);
    QLabel* dir                 = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_DIRECTORY_CHOOSE"]));
    QLabel* game                = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_CHOOSE"]));
    QLabel* backup              = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_BACKUP"]));
    QLabel* clear               = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_CLEAR"]));
    QLabel* recovery            = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_RECOVERY"]));
    QLabel* modLoadType         = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_MOD_TYPE"]));
    QPushButton* dirBTN         = new QPushButton(QString::fromStdString(""));
    QPushButton* gameBTN        = new QPushButton(QString::fromStdString(Core::CONFIG_GAME));
    QPushButton* backupBTN      = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_BACKUP"]));
    QPushButton* clearBTN       = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_CLEAR"]));
    QPushButton* recoveryBTN    = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_RECOVERY"]));
    CSwitchButton* modLoadTypeBTN = new CSwitchButton(Core::lang["LANG_BUTTON_MOD_TYPE_COPY"],
                                                      Core::lang["LANG_BUTTON_MOD_TYPE_MOVE"], false);

    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    copyMode = ModManager::get().get_copy();
    if (!copyMode)
        modLoadTypeBTN->set_target();

    list->addWidget(firstwidget);
    list->addWidget(line);
    list->addWidget(lastwidget);
    firstwidget->setLayout(firstlist);
    lastwidget->setLayout(lastlist);
    list->setStretch(0, 0);
    list->setStretch(1, 0);
    list->setStretch(2, 1);

    firstlist->setAlignment(Qt::AlignTop);
    lastlist->setAlignment(Qt::AlignTop);
    firstlist->setSizeConstraint(QLayout::SetMinimumSize);
    firstlist->addWidget(dir);
    firstlist->addWidget(game);
    firstlist->addWidget(backup);
    firstlist->addWidget(clear);
    firstlist->addWidget(recovery);
    firstlist->addWidget(modLoadType);
    dir->setMinimumHeight(35);
    game->setMinimumHeight(35);
    backup->setMinimumHeight(35);
    clear->setMinimumHeight(35);
    recovery->setMinimumHeight(35);
    modLoadType->setMinimumHeight(35);
    dirBTN->setMinimumHeight(35);
    gameBTN->setMinimumHeight(35);
    backupBTN->setMinimumHeight(35);
    clearBTN->setMinimumHeight(35);
    recoveryBTN->setMinimumHeight(35);
    modLoadTypeBTN->setMinimumHeight(35);
    lastlist->addWidget(dirBTN);
    lastlist->addWidget(gameBTN);
    lastlist->addWidget(backupBTN);
    lastlist->addWidget(clearBTN);
    lastlist->addWidget(recoveryBTN);
    lastlist->addWidget(modLoadTypeBTN);

    dirBTN->setText(QString::fromStdString(Core::CONFIG_GAME_PATH));

    connect(CSettings::get(), &CSettings::save, [this]{
        if (!buffer.isEmpty()) {
            Core::get().save_game_path(buffer.toStdString());
            buffer.clear();
        }
    });
    connect(CSettings::get(), &CSettings::save, [this]{
        if (target) {
            Core::CONFIG_GAME = target->name;
            Core::get().overwriting_config_data();
            target = nullptr;
            Core::get().update_data_from_file();
            ModManager::get().update();
        }
    });
    connect(CSettings::get(), &CSettings::save, [this]{
        ModManager::get().set_copy(copyMode);
    });
    connect(dirBTN,         &QPushButton::clicked, [dirBTN, this]{chooseExe(dirBTN);});
    connect(backupBTN,      &QPushButton::clicked, [this]{createBackup();});
    connect(gameBTN,        &QPushButton::clicked, [gameBTN, this]{chooseGame(gameBTN);});
    connect(clearBTN,       &QPushButton::clicked, [this]{Wait(Core::get().symlink_deliting(););});
    connect(recoveryBTN,    &QPushButton::clicked, [this]{Wait(Core::get().game_recovery(););});
    connect(modLoadTypeBTN, &QPushButton::clicked,  [this]{copyMode = !copyMode;});
}



void setsource::chooseGame (QPushButton* parent) {
    CFastDialog* dialog = new CFastDialog;
    QVBoxLayout* content = new QVBoxLayout;
    addScrollable(dialog->list, content);

    dialog->show();
    content->setAlignment(Qt::AlignTop);
    CLinkTumbler* lastBTN = nullptr;
    for (const auto& entry : std::filesystem::directory_iterator(GAMES)) {
        CLinkTumbler* button = new CLinkTumbler(stc::string::get_name(entry.path().string()), lastBTN);
        lastBTN = button;
        content->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [this, button]{target = button;});
        connect(dialog->apply, &QPushButton::clicked, [this, button, dialog, parent, content]{
            parent->setText(QString::fromStdString(button->name));
            content->removeWidget(target);
            target->setParent(nullptr);
            delete dialog;
        });
    }
}


void setsource::chooseExe (QPushButton* dirBTN) {
    if (Core::CONFIG_GAME != "None" || target) {
        buffer = QFileDialog::getOpenFileName(
            nullptr,
            QString::fromStdString(Core::lang["LANG_LABEL_CHOOSE_GAME_FILE"]),
            "",
            QString::fromStdString(Core::lang["LANG_LABEL_ALL_FILE"] + " (*.exe)"));
        if (!buffer.isEmpty()) {
            dirBTN->setText(buffer);
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R33"]);
}


void setsource::createBackup () {
    if (Core::CONFIG_GAME != "None") {
        if (!Core::CONFIG_GAME_PATH.empty()) {
            Wait(
                Core::get().game_dir_backup();
            );
        }
        else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R40"]);
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R32"]);
}
