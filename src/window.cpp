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
    connect(ObjectList,         &CObjectList::objectChoosed, ContentWidget, &CContentList::updateList);
    connect(ObjectList,         &CObjectList::remove,        ContentWidget, &CContentList::clear);
    connect(ObjectList,         &CObjectList::remove,        this,          &Window::grounding);
    connect(ObjectList,         &CObjectList::remove,        settingsWindow->settings_modules_list->settings_collections,
                                                                            &collections::update_list);
    connect(ObjectList,         &CObjectList::objectChoosed, this,          &Window::updatePointer);
    connect(ObjectList,         &CObjectList::objectChoosed, this,          &Window::inpad_reset);
    connect(inpad,              &CFastDialog::canselClicked, this,          &Window::inpad_reset);
    connect(ContentWidget->dnd, &CDND::launch,               this,          &Window::inpad_reset);
    connect(inpad,              &CInpad::applyClicked,       inpad,         [=]{inpad->application(ObjectList->targetName, ObjectList->TypeTarget);});
    connect(inpad,              &CInpad::applyClicked,       ContentWidget, [=]{ContentWidget->updateList(target, ContentWidget->targetType);});
}

void Window::NewObjectDialog() {
    if (Core::CONFIG_GAME != "None") {
        newObjectDialog = new CNewObjectDialog();
        newObjectDialog->show();
        connect(newObjectDialog, &CFastDialog::applyClicked, [this]{
            ObjectList->newObject(newObjectDialog);
            settingsWindow->settings_modules_list->settings_collections->update_list();
        });
    }
    else
        ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
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
