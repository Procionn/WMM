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
#include "CSmartObject.h"

CSmartObject::CSmartObject(QWidget *parent) : QFrame(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            [this](const QPoint& pos){emit rightClick(pos, this);});
}

bool CSmartObject::is_target() {
    return toggl_condition;
}

void CSmartObject::ChangeOfState () {
    if (toggl_condition)  {
        turnOff();
    }
    else {
        turnOn();
    }
}

void CSmartObject::mousePressEvent (QMouseEvent* event) {
    emit clicked(event, this);
}

void CSmartObject::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
    }
}

void CSmartObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
    }
}

void  CSmartObject::context (const QPoint& pos) {}
