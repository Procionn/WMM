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
#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>
#include <QMainWindow>

#include "../patterns/ERRORdialog.h"
#include "../core.h"
#include "../CONSTANTS.h"


class Window;

class CMenuBar : public QMenuBar
{
    Q_OBJECT
    Window* parent;
public:
    CMenuBar(Window* parent);
signals:
    void settings_click();
    void object_click();
    void content_click();
private slots:
    void assembling_clicked();
    void linking_clicked();
    void reassembly_clicked();
};

#endif // CMENUBAR_H
