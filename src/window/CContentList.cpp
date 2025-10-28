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
#include "mainWidgets.h"

#include "../window/modlist/cobject.h"
#include <QLabel>
#include <QTimer>
#include <wmml.h>

CContentList::CContentList () {
    setMinimumWidth(200);
    QVBoxLayout* BaseContainer = new QVBoxLayout();
    setLayout(BaseContainer);
    siFrame = new CSubInfoFrame(BaseContainer);
    contentList = new CObjectsContainer;
    BaseContainer->addWidget(contentList);
    dnd = new CDND(BaseContainer, Core::lang["LANG_LABEL_DND"]);
}


void CContentList::updateList (CObjectsButton* pointer, bool type) {
    targetName = pointer->name;
    targetType = type;
    clear();
    if (type) sPath = stc::cwmm::ram_preset(targetName);
    else      sPath = stc::cwmm::ram_collection(targetName);
    wmml file(sPath);
    std::vector<wmml::variant> v(file.width());
    bool counter = false;
    for (uint64_t index = 0; file.read(v); ++index) {
        CObject* buttonWidget = new CObject(static_cast<void*>(&v), counter, index);
        contentList->add(buttonWidget);
        buttonWidget->spl1->setSizes(siFrame->spl1->sizes());
        buttonWidget->spl2->setSizes(siFrame->spl2->sizes());

        connect(siFrame->spl1,  &QSplitter::splitterMoved, buttonWidget->spl1, &CSplitter::moveSplitter);
        connect(siFrame->spl2,  &QSplitter::splitterMoved, buttonWidget->spl2, &CSplitter::moveSplitter);
        connect(buttonWidget,   &CObject::ON,     this, &CContentList::changeStatusOn);
        connect(buttonWidget,   &CObject::OFF,    this, &CContentList::changeStatusOff);
        connect(buttonWidget,   &CObject::remove, contentList, &CObjectsContainer::delete_target);
        connect(contentList,    &CObjectsContainer::removed, this, &CContentList::deleting);
    }
}

void CContentList::clear () {
    contentList->clear();
}

void CContentList::changeStatusOn(CObject* toggledElements) {
    wmml file(sPath);
    file.overwriting_sector(toggledElements->index, 4, true);
}
void CContentList::changeStatusOff(CObject* toggledElements) {
    wmml file(sPath);
    file.overwriting_sector(toggledElements->index, 4, false);
}
void CContentList::deleting (CObject* pointer) {
    static CObject* last;
    if (last != pointer) {
        wmml file(sPath);
        file.remove_object(pointer->index);
        delete pointer;
        last = pointer;
    }
}
