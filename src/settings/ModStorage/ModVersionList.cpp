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
#include "ModVersionList.h"
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include "../../methods.h"
#include "../../core.h"

ModVersionObject::ModVersionObject (const QString& version, ModObject* parentWidget)
    : CBaseSmartObject(false) {
    infoEnabled = false;
    QHBoxLayout* lay = new QHBoxLayout(this);
    lay->addWidget(new QLabel(version));
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    setStyleSheet("background-color: #2e2e2e");
}


void ModVersionObject::DELETE () {
    emit remove(this);
}



ModVersionList::ModVersionList (QWidget* parent) : CSmartList(parent) {
    connect(this, &TemplateList::update, this, &ModVersionList::child_status);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void ModVersionList::RMB (const QPoint&, ModVersionObject*) {
    QMenu* contextMenu = new QMenu(this);
    QAction* action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &ModVersionList::deletionSignals);
    contextMenu->exec(QCursor::pos());
}

void ModVersionList::deletionSignals () {
    std::vector<ModVersionObject*> newVector;
    newVector.reserve(childList.size());
    for (auto* target : childList) {
        if (target->toggl_condition)
            target->DELETE();
        else newVector.emplace_back(target);
    }
    childList.clear();
    childList = std::move(newVector);
}


void ModVersionList::child_status () {
    for (const auto* entry : childList) {
        if (!entry->toggl_condition) {
            emit noAllOn();
            return;
        }
    }
    emit allOn();
}


void ModVersionList::set_off () {
    for (auto* entry : childList)
        entry->turnOff();
}


void ModVersionList::set_on () {
    for (auto* entry : childList)
        entry->turnOn();
}
