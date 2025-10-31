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
#ifndef CINPAD_H
#define CINPAD_H

#include "../../patterns/CFastDialog.h"
#include "../../CONSTANTS.h"
#include "../../methods.h"
#include "../../patterns/CScrollWindow.h"
#include "../../window/CObjectsButton.h"
#include "CInpadButton.h"
#include "CInpadList.h"

#include <vector>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <filesystem>

class CInpad : public CFastDialog
{
    Q_OBJECT
public:

    bool targetType = false;
    bool* callType;
    CInpadList* newObjectList;
    CObjectsButton** target;

    CInpad(bool& type);
    void reset();
    void application(const std::string& targetName, const bool targetType);

private:
    std::vector<CInpadButton*> vlist;
    bool vector = false;
    bool count_type = false;
    QMenu* menu;
    QAction* presets;

    void render();
    void reader();
    void presets_directory_scaner(const std::vector<std::string>& existsElements);
    void mods_scaner(const std::vector<std::string>& existsElements);
    void distributor();
    bool not_exists(const std::vector<std::string>& existsElements, const std::string& str);
    void search(const QString&, const bool);
    void coloring();

private slots:
    void search_slot(const QString&);
};

#endif // CINPAD_H
