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
#include "CSearchWidget.h"

#include <QPushButton>
#include <QHBoxLayout>

CSearchWidget::CSearchWidget () {
    QHBoxLayout* lay = new QHBoxLayout(this);
    searchTab = new QLineEdit;
    QPushButton* closeButton = new QPushButton("X");
    closeButton->setFixedSize(20, 20);
    lay->addWidget(searchTab);
    lay->addWidget(closeButton);
    searchTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(2);
    setMidLineWidth(2);
    setStyleSheet("background-color: #2f3438;");
    searchTab->setStyleSheet("background-color: #202326");
    QFrame::hide();

    connect(searchTab,   &QLineEdit::textEdited, [this](const QString& str){emit search_updated(str.toStdString());});
    connect(closeButton, &QPushButton::clicked, this, &CSearchWidget::hide);
}



void CSearchWidget::hide () {
    searchTab->clear();
    QFrame::hide();
}



void CSearchWidget::show () {
    QFrame::show();
    searchTab->setFocus();
}
