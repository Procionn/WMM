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
#include "CFastDialog.h"

#include <QVBoxLayout>
#include "../core.h"

CFastDialog::CFastDialog () {
    setMinimumHeight(230);
    setMinimumWidth(350);
    // setModal(Qt::WindowModal);
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    DialogButtonBox = new QHBoxLayout();
    apply = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_APPLY"]), this);
    cansel = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_CANSEL"]), this);
    list = new QWidget();
    list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dialogLayout->addWidget(list);
    dialogLayout->addLayout(DialogButtonBox);
    DialogButtonBox->addWidget(apply);
    DialogButtonBox->addWidget(cansel);
    apply->setDefault(true);
    connect(apply,  &QPushButton::clicked, [this]{emit applyClicked();});
    connect(cansel, &QPushButton::clicked, [this]{
        reject();
        emit canselClicked();
        if (autodeleting)
            delete this;
    });
}
