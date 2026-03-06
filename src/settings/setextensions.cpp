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
#include "setextensions.h"
#include "../methods.h"
#include "../patterns/CLinkTumbler.h"
#include "../patterns/CScrollWindow.h"
#include "../plugins/PluginInterface.h"
#include "../plugins/PluginLoader.h"
#include <QApplication>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPluginLoader>
#include <QString>
#include <QVBoxLayout>


setextensions::setextensions() {
    list = new QVBoxLayout;
    list->setAlignment(Qt::AlignTop);
    addScrollable(this, list);
    generate_buttons();
}

setextensions::~setextensions() {
    clear_list();
}


void setextensions::clear_list() {
    if (!expansionList.empty()) {
        for (auto* entry : expansionList)
            delete entry;
        expansionList.clear();
    }
}


void setextensions::generate_buttons() {
    clear_list();
    for (auto* plugin : PluginLoader::get_plugins_list()) {
        PluginIco* widget = new PluginIco(plugin);
        list->addWidget(widget);
        expansionList.emplace_back(widget);
    }
}



PluginIco::PluginIco (PluginInterface* chld) : child(chld) {
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);
    vLay = new QVBoxLayout(this);
    vLay->addWidget(name = new PCLabel(chld->name()));
    vLay->addWidget(description = new PCLabel(chld->description()));
    connect(name, &PCLabel::clicked, this, &PluginIco::mousePressEvent);
    connect(description, &PCLabel::clicked, this, &PluginIco::mousePressEvent);
}


void PluginIco::mousePressEvent (QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        child->main();
}



PCLabel::PCLabel (const QString& str) : QLabel(str) {}
void PCLabel::mousePressEvent (QMouseEvent* event) { emit clicked(event); }
