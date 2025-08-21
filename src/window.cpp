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
#include "window.h"

#include "core.h"
#include "CONSTANTS.h"
#include <string>
#include <QApplication>
#include <QSplitter>
// #include <QDockWidget>

Window::Window () {
    CMenuBar* menu = new CMenuBar(this);
    QSplitter* BaseSplitter = new QSplitter();
    ObjectList = new CObjectList;
    ContentWidget = new CContentList();
    inpad = new CInpad(ObjectList->TypeTarget);
    settingsWindow = new CSettings;

    setMenuBar(menu);
    setCentralWidget(BaseSplitter);
    BaseSplitter->addWidget(ObjectList);
    BaseSplitter->addWidget(ContentWidget);

    ObjectList->resize(200, 0);
    ContentWidget->resize(1000, 0);
    resize(1200, 800);
    show();

    inpad->target = &target;

    connect(settingsWindow->settings_modules_list->settings_collections,
            &collections::import_completed,                  this,          &Window::import_completed);
    connect(menu,               &CMenuBar::content_click,    this,          &Window::inpadShow);
    connect(menu,               &CMenuBar::settings_click,   this,          &Window::settings);
    connect(menu,               &CMenuBar::object_click,     this,          &Window::NewObjectDialog);
    connect(ObjectList,         &CObjectList::remove,        this,          &Window::removing);
    connect(ObjectList,         &CObjectList::objectChoosed, this,          &Window::objectChoosing);
    connect(inpad,              &CFastDialog::canselClicked, this,          &Window::inpad_reset);
    connect(ContentWidget->dnd, &CDND::launch,               this,          &Window::inpad_reset);
    connect(inpad,              &CInpad::applyClicked,       this,          &Window::applyClicking);
}

void Window::NewObjectDialog() {
    if (Core::CONFIG_GAME != "None") {
        newObjectDialog = new CNewObjectDialog();
        newObjectDialog->show();
        connect(newObjectDialog, &CNewObjectDialog::success, [this]{
            ObjectList->CreteObject(newObjectDialog->name);
            settingsWindow->settings_modules_list->settings_collections->update_list();
        });
    }
    else
        ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
}


void Window::removing (CObjectsButton* pointer) {
    ContentWidget->clear();
    grounding(pointer);
    settingsWindow->settings_modules_list->settings_collections->update_list();
}


void Window::objectChoosing (CObjectsButton* pointer, bool type) {
    ContentWidget->updateList(pointer, type);
    updatePointer(pointer);
    inpad_reset();
}


void Window::applyClicking () {
    inpad->application(ObjectList->targetName, ObjectList->TypeTarget);
    ContentWidget->updateList(target, ContentWidget->targetType);
}


void Window::import_completed() {
    ObjectList->updateList();
    ObjectList->render();
}


void Window::settings() {
    settingsWindow->show();
}

void Window::updatePointer(CObjectsButton* pointer) {
    target = pointer;
}

void Window::grounding (CObjectsButton* pointer) {
    if (target && pointer->name == target->name)
        target = nullptr;
}

void Window::inpadShow () {
    if (target != nullptr)
        inpad->show();
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R35"]);
}


void Window::inpad_reset () {
    inpad->reset();
}


void Window::closeEvent (QCloseEvent* event) {
    QApplication::exit();
}


Window::~Window () {
    delete settingsWindow;
}
