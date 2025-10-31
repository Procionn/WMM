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
#ifndef COBJECTSBUTTON_H
#define COBJECTSBUTTON_H

#include "../patterns/CLinkTumbler.h"

#include <string>
#include <QWidget>
#include <QMenu>


class CObjectsButton : public CLinkTumbler
{
    Q_OBJECT
    CObjectsButton* link = nullptr;

public:
    bool type;
    CObjectsButton(const std::string& name = "", CObjectsButton* linked = nullptr);

private slots:
    void context(const QPoint &pos);
    void DELETE();

signals:
    void remove(CObjectsButton* button);
};

#endif // COBJECTSBUTTON_H
