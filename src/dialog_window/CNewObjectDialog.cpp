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
#include "CNewObjectDialog.h"
#include "../core.h"
#include "../patterns/ERRORdialog.h"

CNewObjectDialog::CNewObjectDialog () {
    QHBoxLayout* glist = new QHBoxLayout;
    list->setLayout(glist);
    glist->setAlignment(Qt::AlignTop);
    glist->setContentsMargins(15, 15, 0, 0);
    glist->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_NAME"])));
    nameTab = new QLineEdit;
    glist->addWidget(nameTab);
    connect(apply, &QPushButton::clicked, this, &CNewObjectDialog::test);
}


void CNewObjectDialog::test () {
    std::string tab = nameTab->text().toStdString();
#ifdef __linux__
    std::string separator = "/";
#elif WIN64
    std::string separator = "<>:\"/\\|?*";
#endif
    if (tab.find_last_of(separator) == std::string::npos) {
        name = std::move(tab);
        emit success();
        delete this;
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_PATH_RULES"] + separator);
}
