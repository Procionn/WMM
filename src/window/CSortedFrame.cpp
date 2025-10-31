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
#include "CSortedFrame.h"

CSortedFrame::CSortedFrame(const std::string& string) :
    QLabel(QString::fromStdString(string + spacing)), name(string) {}



void CSortedFrame::mousePressEvent (QMouseEvent*) {
    switch (state) {
    case  0:
    case -1:
        set_up();
        break;
    case  1:
        set_down();
        break;
    }
}



void CSortedFrame::set_up () {
    state = 1;
    setText(QString::fromStdString(name + " " + v));
    emit up();
}



void CSortedFrame::set_down () {
    state = -1;
    setText(QString::fromStdString(name + " " + n));
    emit down();
}



void CSortedFrame::reset () {
    state = 0;
    setText(QString::fromStdString(name + spacing));
}
