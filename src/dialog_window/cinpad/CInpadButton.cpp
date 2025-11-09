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
#include "CInpadButton.h"
#include <QVBoxLayout>
#include <QLabel>

CInpadButton::CInpadButton(const std::string& setName, const bool& type, bool& counter) :
    type(type), name(setName) {

    QVBoxLayout* list = new QVBoxLayout(this);
    QLabel* label = new QLabel(QString::fromStdString(setName));
    list->addWidget(label);
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
    setMinimumHeight(35);
    if (counter)
        setStyleSheet(QString::fromStdString(untoggledColor1));
    else
        setStyleSheet(QString::fromStdString(untoggledColor2));

    count_type = counter;
    hide();
}


void CInpadButton::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(untoggledColor1));
        else
            setStyleSheet(QString::fromStdString(untoggledColor2));
    }
}

void CInpadButton::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(toggledColor1));
        else
            setStyleSheet(QString::fromStdString(toggledColor2));
    }
}

bool CInpadButton::is_target () {
    return toggl_condition;
}

std::string CInpadButton::get_name () {
    return name;
}

void CInpadButton::set_style (const bool type) {
    if (toggl_condition) {
        if (type)
            setStyleSheet(QString::fromStdString(toggledColor1));
        else
            setStyleSheet(QString::fromStdString(toggledColor2));
    }
    else {
        if (type)
            setStyleSheet(QString::fromStdString(untoggledColor1));
        else
            setStyleSheet(QString::fromStdString(untoggledColor2));
    }
    count_type = type;
}

bool CInpadButton::get_style () {
    return count_type;
}
