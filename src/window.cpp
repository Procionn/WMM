#include "window.h"
#include "core.h"
#include "CONSTANTS.h"
#include "settings/settings.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <filesystem>
#include <QString>

// #include <QDockWidget>
#include <QPushButton>
#include <QSplitter>
#include <QMenuBar>

Window::Window () {
    CMenuBar* menu = new CMenuBar(this);
    QSplitter* BaseSplitter = new QSplitter();
    ObjectList = new CObjectList;
    ContentWidget = new CContentList();
    inpad = new CInpad(ObjectList->TypeTarget);

    setMenuBar(menu);
    setCentralWidget(BaseSplitter);
    BaseSplitter->addWidget(ObjectList);
    BaseSplitter->addWidget(ContentWidget);

    ObjectList->resize(200, 0);
    ContentWidget->resize(1000, 0);

    inpad->target = &target;

    connect(menu,               &CMenuBar::content_click,    this,          &Window::inpadShow);
    connect(menu,               &CMenuBar::settings_click,   this,          &Window::settings);
    connect(menu,               &CMenuBar::object_click,     this,          &Window::NewObjectDialog);
    connect(ObjectList,         &CObjectList::objectChoosed, ContentWidget, &CContentList::updateList);
    connect(ObjectList,         &CObjectList::remove,        ContentWidget, &CContentList::clear);
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
        connect(newObjectDialog, &CFastDialog::applyClicked, ObjectList, [=]{ObjectList->newObject(newObjectDialog);});
    }
    else
        ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
}

void Window::settings() {
    CSettings* set = new CSettings;
}

void Window::updatePointer(CObjectsButton* pointer) {
    target = pointer;
}

void Window::inpadShow () {
    if (target != nullptr)
        inpad->show();
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R35"]);
}


void Window::inpad_reset () {
    inpad->reset();
}
