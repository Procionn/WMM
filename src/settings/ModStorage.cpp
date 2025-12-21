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
#include "ModStorage.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include "ModStorage/ModStorageList.h"

ModStorage::ModStorage() {
    QVBoxLayout* mainLay = new QVBoxLayout(this);
    QLineEdit* searchTab = new QLineEdit;
    ModStorageList* objectLay = new ModStorageList;
    mainLay->addWidget(searchTab);
    mainLay->addWidget(objectLay);

    connect(searchTab, &QLineEdit::textEdited, objectLay, &ModStorageList::search);
}
