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
#include "templatelist.h"

TemplateList::TemplateList (QWidget* parent) : QScrollArea(parent){
    list = new QVBoxLayout;
    QWidget* scrolledWidget = new QWidget;

    list->setAlignment(Qt::AlignTop);
    setWidgetResizable(true);
    setWidget(scrolledWidget);
    scrolledWidget->setLayout(list);
}

void TemplateList::connecting (CSmartObject* linkable) {
    connect(linkable, &CSmartObject::clicked,    [this]
        (QMouseEvent* event, CSmartObject* target) {
        controller(event, target);
    });
    connect(linkable, &CSmartObject::rightClick, [this]
        (const QPoint& pos, CSmartObject* target) {
        rightClickController(pos, target);
    });
}

void TemplateList::updated () {
    emit update();
}

void TemplateList::reseted () {
    emit reseting();
}
