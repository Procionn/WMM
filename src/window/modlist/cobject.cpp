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
#include "../../ModManager.h"
#include <wmml.h>

CObject::CObject(const void* v, bool& counter, const uint64_t& index) :
    index(index){
    const auto* c = static_cast<const std::vector<wmml::variant>*>(v);
    Box = new QHBoxLayout(this);


    Lname = new QLabel(QString::fromStdString(std::get<std::string>(c->at(0))));
    Lversion = new CVersion(std::get<std::string>(c->at(1)), this);
    if (std::get<bool>(c->at(2)))
         Ltype = new QLabel("Mod");
    else Ltype = new QLabel("Collection");
    switcher = new CSwitchButton;


    name = std::get<std::string>(c->at(0));
    version = std::get<std::string>(c->at(1));
    type = std::get<bool>(c->at(2));
    id = std::get<uint64_t>(c->at(3));
    switcher->setTheme("orange");
    if (std::get<bool>(c->at(4)))   switcher->isTarget(true);
    else                            switcher->isTarget(false);

    switcher->setMinimumWidth(70);
    switcher->setMinimumHeight(20);
    setMinimumHeight(50);

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
    count_type = counter;
}


void CObject::context (const QPoint& pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction* action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &CObject::DELETE);
    if (type) {
        QAction* action2 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_INFO"]));
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
            setStyleSheet(QString::fromStdString(untoggledColor1 + border));
        else
            setStyleSheet(QString::fromStdString(untoggledColor2 + border));
    }
}

void CObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(toggledColor1 + border));
        else
            setStyleSheet(QString::fromStdString(toggledColor2 + border));
    }
}

void CObject::set_style (const bool type) {
    if (type)
        setStyleSheet(QString::fromStdString(untoggledColor1));
    else
        setStyleSheet(QString::fromStdString(untoggledColor2));
    count_type = type;
}

bool CObject::get_style () {
    return count_type;
}





CVersion::CVersion (const std::string& version, CObject* parent) :
    parent(parent), QLabel(QString::fromStdString(version)) {
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CVersion::context);
}



void CVersion::context (const QPoint& pos) {
    if (!parent->type) return;
    QMenu* contextMenu = new QMenu(this);
    const auto container = ModManager::get().all_versions_list(parent->id);
    std::string_view entry;
    int i = container.size();
    for (--i; i >= 0; --i) {
        entry = container.at(i);
        QAction* action = new QAction(entry.data());
        contextMenu->addAction(action);
        connect(action, &QAction::triggered, this, [entry, this]{
            if (entry == parent->version)
                return;
            setText(entry.data());
            parent->version = entry;
            emit version_changed(entry, parent->index);
        });
    }
    contextMenu->exec(mapToGlobal(pos));
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
