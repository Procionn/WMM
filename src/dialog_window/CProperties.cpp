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
#include "CProperties.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include "../core.h"
#include "../window/modlist/cobject.h"

CProperties::CProperties(signed char base) {
    QGridLayout* layout = new QGridLayout(list);
    layout->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_PRIORITY"])));
    priority = new QSpinBox();
    priority->setRange(-127, 127);
    priority->setValue((signed int)(base));
    layout->addWidget(priority, 0, 2);

    connect(apply, &QPushButton::clicked, this, &CProperties::appled);
    show();
}

void CProperties::appled() {
    emit complited(priority->value());
    delete this;
}
