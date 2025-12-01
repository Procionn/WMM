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
    object = this;
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
    connect(cansel, &QPushButton::clicked, [this]{this->reject();});
    connect(accept, &QPushButton::clicked, [this]{emit save();});

#define X(button, class_, object, lang_str) \
    connect(sobjects->button, &CLinkTumbler::toggled, [this]{  \
        settings_modules_list->show(qobject_cast<QWidget*>(settings_modules_list->object));   \
    });
    SETTING_SPAGES()
#undef X


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



CSettings* CSettings::get () {
    return object;
}





SObjects::SObjects () {
    list = new QVBoxLayout;
    list->setAlignment(Qt::AlignTop);
    addScrollable(this, list);
    CLinkTumbler* last = nullptr;

#define X(button, class_, object, lang_str) \
    button = new CLinkTumbler(Core::lang[lang_str], last); \
    button->SetLeftAlignment(true);   \
    button->setMinimumHeight(35);     \
    list->addWidget(button);  \
    last = button;

    SETTING_SPAGES()
#undef X
    
    extensions->hide();
}





SList::SList () {
    QVBoxLayout* list   = new QVBoxLayout(this);

#define X(button, class_, object, lang_str) \
    object = new class_; \
    list->addWidget(object);    \
    pages.emplace_back(object); \
    object->hide();

    SETTING_SPAGES()
#undef X
    settings_source->show();
}

void SList::show(QWidget* displayed) {
    for (auto* entry : pages) {
        if (entry == displayed)
            entry->show();
        else
            entry->hide();
    }
}
