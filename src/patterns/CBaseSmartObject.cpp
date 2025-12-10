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
#include "CBaseSmartObject.h"

#include <QMenu>
#include "../core.h"

CBaseSmartObject::CBaseSmartObject(const bool type, QWidget* parent)
    : CSmartObject(parent), count_type(type) {}


void CBaseSmartObject::context (const QPoint& pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction* action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, [this]{DELETE();});
    if (infoEnabled) {
        QAction* action2 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_INFO"]));
        connect(action2, &QAction::triggered, [this]{INFO();});
    }
    contextMenu->exec(this->mapToGlobal(pos));
}

void CBaseSmartObject::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(dataString + untoggledColor1));
        else
            setStyleSheet(QString::fromStdString(dataString + untoggledColor2));
    }
}

void CBaseSmartObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(dataString + toggledColor1));
        else
            setStyleSheet(QString::fromStdString(dataString + toggledColor2));
    }
}

void CBaseSmartObject::set_style (const bool type) {
    if (type)
        setStyleSheet(QString::fromStdString(dataString + untoggledColor1));
    else
        setStyleSheet(QString::fromStdString(dataString + untoggledColor2));
    count_type = type;
}

bool CBaseSmartObject::get_style () {
    return count_type;
}
