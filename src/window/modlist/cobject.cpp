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
#include "cobject.h"

CObject::CObject(const std::vector<wmml::variant>& v, bool& counter, const u_int64_t& index) {
    Box = new QHBoxLayout(this);


    Lname = new QLabel(QString::fromStdString(std::get<std::string>(v[0])));
    Lversion = new QLabel(QString::fromStdString(std::get<std::string>(v[1])));
    if (std::get<bool>(v[2]))
         Ltype = new QLabel("Mod");
    else Ltype = new QLabel("Collection");
    switcher = new CSwitchButton;


    name = std::get<std::string>(v[0]);
    version = std::get<std::string>(v[1]);
    type = std::get<bool>(v[2]);
    id = std::get<u_int64_t>(v[3]);
    switcher->setTheme("orange");
    if (std::get<bool>(v[4])) switcher->isTarget(true);
    else             switcher->isTarget(false);
    this->index = index;

    switcher->setMinimumWidth(70);
    switcher->setMinimumHeight(20);

    Lname->resize(400, 0);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);
    spl1 = new CSplitter(Box);
    spl2 = new CSplitter(Box);
    spl1->addWidget(Lname);
    spl1->addWidget(spl2);
    spl2->addWidget(Lversion);
    spl2->addWidget(Ltype);
    Box->addWidget(switcher);

    connect(switcher, &CSwitchButton::toggled,   this, [=]{emit ON(this);});
    connect(switcher, &CSwitchButton::untoggled, this, [=]{emit OFF(this);});
    if (counter) {
        setStyleSheet(QString::fromStdString(untoggledColor1 + border));
        counter = false;
    }
    else {
        setStyleSheet(QString::fromStdString(untoggledColor2 + border));
        counter = true;
    }
    count_type = counter;
}


void CObject::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &CObject::DELETE);
    if (type) {
        QAction *action2 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_INFO"]));
        connect(action2, &QAction::triggered, this, &CObject::INFO);
    }
    contextMenu->exec(this->mapToGlobal(pos));
}

void CObject::DELETE() {
    emit remove(this);
}

void CObject::INFO() {
    stc::net::openURL(stc::cwmm::modsURL(std::to_string(id)));
}

void CObject::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(untoggledColor2 + border));
        else
            setStyleSheet(QString::fromStdString(untoggledColor1 + border));
    }
}

void CObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(toggledColor2 + border));
        else
            setStyleSheet(QString::fromStdString(toggledColor1 + border));
    }
}




CSplitter::CSplitter(QHBoxLayout* parent) {
    parent->addWidget(this);
}
void CSplitter::moveSplitter(int pos, int index){
    if(index >= sizes().size()) return;
    bool oldState = blockSignals(true);
    QSplitter::moveSplitter(pos, index);
    blockSignals(oldState);
}
