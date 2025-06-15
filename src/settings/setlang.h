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
#ifndef SETLANG_H
#define SETLANG_H

#include "../patterns/CScrollWindow.h"
#include "../patterns/CFastDialog.h"
#include "../patterns/CLinkTumbler.h"

#include <QWidget>
#include <QPushButton>

class setlang : public QWidget
{
    Q_OBJECT
public:
    CLinkTumbler* target = nullptr;
public:
    setlang();
private slots:
    void chooseLang(QPushButton* parent);
};

#endif // SETLANG_H
