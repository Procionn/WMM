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

    QFrame* siFrame = new QFrame; // subinfo Frame
    QHBoxLayout* splitterBox = new QHBoxLayout(siFrame);
    BaseContainer->addWidget(siFrame);
    siFrame->setStyleSheet("background-color: #444b50; border-radius: 10px;");

    spl1 = new QSplitter;
    spl2 = new QSplitter;
    QLabel* lblName     = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_NAME"]));
    QLabel* lblVersion  = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"]));
    QLabel* lblType     = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_TYPE"]));
    QLabel* lblSwitcher = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_SWITCHER"]));

    splitterBox->addWidget(spl1);
    splitterBox->addWidget(spl2);
    splitterBox->addWidget(lblSwitcher);
    lblName->resize(400, 0);
    spl1->addWidget(lblName);
    spl1->addWidget(spl2);
    spl2->addWidget(lblVersion);
    spl2->addWidget(lblType);

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


        // Crutch. It will need to be fixed
        auto splz = [](QSplitter* spl, double constant) -> double {
            double sz1 = spl->size().width();
            double sz2 = spl->size().height();
            return sz1 / (sz2 / constant);
        };

        static double sz31 = splz(spl1, 13);
        static double sz32 = splz(spl2, 10.5);

        QTimer::singleShot(0, buttonWidget, [=]() {
            buttonWidget->spl1->moveSplitter(sz31, 1);
            buttonWidget->spl2->moveSplitter(sz32, 1);
        });


        connect(spl1, &QSplitter::splitterMoved, [&](int pos){sz31 = pos;});
        connect(spl2, &QSplitter::splitterMoved, [&](int pos){sz32 = pos;});
        ////////////////////////////

        connect(spl1, &QSplitter::splitterMoved, buttonWidget->spl1, &CSplitter::moveSplitter);
        connect(spl2, &QSplitter::splitterMoved, buttonWidget->spl2, &CSplitter::moveSplitter);
        connect(buttonWidget, &CObject::ON,     this, &CContentList::changeStatusOn);
        connect(buttonWidget, &CObject::OFF,    this, &CContentList::changeStatusOff);
        connect(buttonWidget, &CObject::remove, contentList, &CObjectsContainer::delete_target);
        connect(contentList,  &CObjectsContainer::removed, this, &CContentList::deleting);
    }
}

void CContentList::clear () {
    contentList->clear();
}

void CContentList::changeStatusOn(CObject* toggledElements) {
    wmml file(sPath);
    auto bol = true;
    file.overwriting_sector(toggledElements->index, 4, bol);
}
void CContentList::changeStatusOff(CObject* toggledElements) {
    wmml file(sPath);
    auto bol = false;
    file.overwriting_sector(toggledElements->index, 4, bol);
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
