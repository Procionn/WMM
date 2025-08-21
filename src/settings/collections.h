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
#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QFrame>
#include "../patterns/CLinkTumbler.h"

class SettingsBox : public QFrame
{
public:
    bool monolith = false;
    bool online = false;

    SettingsBox();
};



class collections : public QWidget
{
    Q_OBJECT
    QLabel* mods;
    QLabel* presets;
    QLabel* allMods;
protected:

    CToggledButton* target = nullptr;
    QVBoxLayout* collections_list;
    QVBoxLayout* right_list;
    SettingsBox* settingsBox;

    void update_collection_info(CToggledButton* target);

public:
    collections();

signals:
    void import_completed();

public slots:
    void update_list();

private slots:
    void exporting();
    void importing();
};

#endif // COLLECTIONS_H
