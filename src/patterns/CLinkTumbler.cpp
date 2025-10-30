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
#include "CLinkTumbler.h"

CLinkTumbler::CLinkTumbler(const std::string& name, CLinkTumbler* linked) : CToggledButton(name){
    if (linked != nullptr) {
        link = linked;
        connect(this, &CLinkTumbler::clicked, linked, &CLinkTumbler::reset);
        connect(linked, &CLinkTumbler::clicked, this, &CLinkTumbler::reset);
        while (linked->link != nullptr) {
            linked = linked->link;
            connect(this, &CLinkTumbler::clicked, linked, &CLinkTumbler::reset);
            connect(linked, &CLinkTumbler::clicked, this, &CLinkTumbler::reset);
        }
    }
}

void CLinkTumbler::ChangeOfState() {
    if (!selected) {
        emit toggled(this, name);
        selected = true;
        setStyleSheet(QString::fromStdString(toggledColor));
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + toggledColor));
        else setStyleSheet(QString::fromStdString(toggledColor));
    }
}
void CLinkTumbler::reset() {
    if (selected) {
        selected = false;
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + untoggledColor));
        else setStyleSheet(QString::fromStdString(untoggledColor));
    }
}
