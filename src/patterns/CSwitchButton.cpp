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
#include "CSwitchButton.h"

CSwitchButton::CSwitchButton(const std::string& firstName, const std::string& secondName, const bool& colored)
    : firstName(QString::fromStdString(firstName)), secondName(QString::fromStdString(secondName)),
      colored(colored), CToggledButton(firstName) {}

void CSwitchButton::ChangeOfState() {
    if (selected) {
        selected = false;
        this->setText(firstName);
        if (colored)
            setStyleSheet(QString::fromStdString(untoggledColor));
        emit untoggled(this);
    }
    else {
        selected = true;
        this->setText(secondName);
        if (colored)
            setStyleSheet(QString::fromStdString(toggledColor));
        emit toggled(this);
    }
}
