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
#include "setsupport.h"

#include <QVBoxLayout>
#include "../core.h"

setsupport::setsupport() {
    QVBoxLayout* list = new QVBoxLayout(this);
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_WMM"])));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"] + VERSION)));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_USED_LIBS"])));
    list->addWidget(new QLabel("\tQt Framework"));
    list->addWidget(new QLabel("\tLibarchive"));
    list->addWidget(new QLabel("\tWMML"));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CREDITS"])));
    list->addWidget(new QLabel("\tQt Framework\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_QT"])));
    list->addWidget(new QLabel("\tLibarchive\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_LIBARCHIVE"])));
    list->addWidget(new QLabel("\tJekkks\t\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_JEKKKS"])));
    list->addWidget(new QLabel());
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LABEL_CREDITS_MORE_INFO"])));

    list->setAlignment(Qt::AlignTop);
}

