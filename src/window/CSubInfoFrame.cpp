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
#include "CSubInfoFrame.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include "CSortedFrame.h"
#include "../core.h"
#include "modlist/cobjectscontainer.h"

CSubInfoFrame::CSubInfoFrame (QVBoxLayout* parent) {
    QScrollArea* scroll = new QScrollArea;
    QWidget* scrolledWidget = new QWidget;
    scroll->setWidgetResizable(true);
    scroll->setWidget(scrolledWidget);
    scrolledWidget->setLayout(this);
    parent->addWidget(scroll);

    QFrame* siFrame = new QFrame(); // SubInfo Frame
    QHBoxLayout* splitterBox = new QHBoxLayout(siFrame);
    addWidget(siFrame);

    CSortedFrame* lblName     = new CSortedFrame(Core::lang["LANG_LABEL_NAME"]);
    CSortedFrame* lblVersion  = new CSortedFrame(Core::lang["LANG_LABEL_VERSION"]);
    CSortedFrame* lblType     = new CSortedFrame(Core::lang["LANG_LABEL_TYPE"]);
    CSortedFrame* lblSwitcher = new CSortedFrame(Core::lang["LANG_LABEL_SWITCHER"]);
    list.emplace_back(lblName);
    list.emplace_back(lblVersion);
    list.emplace_back(lblType);
    list.emplace_back(lblSwitcher);
    lblName->resize(400, 0);

    spl1 = new QSplitter;
    spl2 = new QSplitter;

    splitterBox->addWidget(spl1);
    spl1->addWidget(lblName);
    spl1->addWidget(spl2);
    spl2->addWidget(lblVersion);
    spl2->addWidget(lblType);
    splitterBox->addWidget(lblSwitcher);

    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    siFrame->setStyleSheet("background-color: #444b50; border-radius: 10px;");

    connect(lblName,    &CSortedFrame::up,     [this, lblName]{
        emit filter_changed(CObjectsContainer::NAME_T);
        reset(lblName);
    });
    connect(lblName,    &CSortedFrame::down,   [this, lblName]{
        emit filter_changed(CObjectsContainer::NAME_F);
        reset(lblName);
    });
    connect(lblVersion, &CSortedFrame::up,     [this, lblVersion]{
        emit filter_changed(CObjectsContainer::VERSION_T);
        reset(lblVersion);
    });
    connect(lblVersion, &CSortedFrame::down,   [this, lblVersion]{
        emit filter_changed(CObjectsContainer::VERSION_F);
        reset(lblVersion);
    });
    connect(lblType,    &CSortedFrame::up,     [this, lblType]{
        emit filter_changed(CObjectsContainer::TYPE_T);
        reset(lblType);
    });
    connect(lblType,    &CSortedFrame::down,   [this, lblType]{
        emit filter_changed(CObjectsContainer::TYPE_F);
        reset(lblType);
    });
    connect(lblSwitcher,&CSortedFrame::up,     [this, lblSwitcher]{
        emit filter_changed(CObjectsContainer::CONDITION_T);
        reset(lblSwitcher);
    });
    connect(lblSwitcher,&CSortedFrame::down,   [this, lblSwitcher]{
        emit filter_changed(CObjectsContainer::CONDITION_F);
        reset(lblSwitcher);
    });

}



void CSubInfoFrame::reset (CSortedFrame* target) {
    for (auto* entry : list)
        if (entry != target)
            entry->reset();
}
