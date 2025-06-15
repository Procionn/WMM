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
#include "ERRORdialog.h"

#include "../core.h"

ERRORdialog::ERRORdialog (const std::string& label) {
    show();
    apply->hide();
    QLayout* tempLay = new QVBoxLayout(list);
    tempLay->setAlignment(Qt::AlignTop);
    QLabel* lbl = new QLabel(QString::fromStdString(label));
    lbl->setWordWrap(true);
    tempLay->addWidget(lbl);
}





FatalError::FatalError (const std::string& error, bool fatality) {
    this->fatality = fatality;
    show();
    apply->hide();

    QVBoxLayout* tempLay = new QVBoxLayout(list);
    QLabel* label = new QLabel(QString::fromStdString(error));
    QLabel* errorLable = new QLabel("Fatal Error!");
    QPushButton* resetButton = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_RESET"]));

    if (fatality)
        tempLay->addWidget(errorLable);
    else
        delete errorLable;
    tempLay->setAlignment(Qt::AlignTop);
    label->setWordWrap(true);
    tempLay->addWidget(label);

    DialogButtonBox->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, [&]{
        delete this;
        throw new reset;
    });
}


FatalError::~FatalError () {
    if (fatality)
        abort();
}
