#include "window.h"
#include "lang.h"
#include "widgetpattern.h"
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
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QAction* settings = new QAction(QString::fromStdString(Lang::LANG_BUTTON_SETTINGS), this);
    connect(settings, &QAction::triggered, this, &Window::settings);
    menuBar->addAction(settings);
    
    QAction* object = new QAction(QString::fromStdString(Lang::LANG_BUTTON_NEW_OBJECT), this);
    connect(object, &QAction::triggered, this, &Window::NewObjectDialog);
    menuBar->addAction(object);
    
    QAction* content = new QAction(QString::fromStdString(Lang::LANG_BUTTON_ADD_COMPONENTS), this);
    menuBar->addAction(content);
    
    QAction* assembling = new QAction(QString::fromStdString(Lang::LANG_BUTTON_ASSEMBLING), this);
    connect(assembling, &QAction::triggered,    this,   [=]{if (target != nullptr) {
                                                                if (ObjectList->TypeTarget)
                                                                    ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R30);
                                                                else  configurator::collector(target->name, ObjectList->TypeTarget);
                                                            }
                                                            else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R31);});
    menuBar->addAction(assembling);
    QAction* reassembly = new QAction(QString::fromStdString(Lang::LANG_BUTTON_REASSEMBLY), this);
    connect(reassembly, &QAction::triggered,    this,   [=]{if (target != nullptr) {
                                                                if (ObjectList->TypeTarget)
                                                                    ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R30);
                                                                else {
                                                                    std::filesystem::remove_all(COLLECTIONS + CConfigs::CONFIG_GAME + "/" + target->name);
                                                                    configurator::collector(target->name, ObjectList->TypeTarget);
                                                                }
                                                            }
                                                            else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R31);});
    menuBar->addAction(reassembly);
    QAction* linking = new QAction(QString::fromStdString(Lang::LANG_BUTTON_LINKING), this);
    connect(linking,    &QAction::triggered,    this,   [=]{if (target != nullptr && !ObjectList->TypeTarget) {
                                                                if (CConfigs::CONFIG_GAME_PATH != "") {
                                                                    CGameConfig cgc;
                                                                    cgc.symlink_deliting();
                                                                    cgc.symlink_creating(target->name);
                                                                }
                                                            }
                                                            else if (ObjectList->TypeTarget)
                                                                 ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R30);
                                                            else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R36);
                                                            });
    menuBar->addAction(linking);
    
    QSplitter* BaseSplitter = new QSplitter();
    setCentralWidget(BaseSplitter);
    ObjectList = new CObjectList;
    BaseSplitter->addWidget(ObjectList);
    ObjectList->resize(200, 0);
    ContentWidget = new CContentList();
    BaseSplitter->addWidget(ContentWidget);
    ContentWidget->resize(1000, 0);
    inpad = new CInpad(ObjectList->TypeTarget);
    inpad->target = &target;
    connect(content,            &QAction::triggered,         this,          &Window::inpadShow);
    connect(ObjectList,         &CObjectList::objectChoosed, ContentWidget, &CContentList::updateList);
    connect(ObjectList,         &CObjectList::remove,        ContentWidget, &CContentList::clear);
    connect(ObjectList,         &CObjectList::objectChoosed, this,          &Window::updatePointer);
    connect(ObjectList,         &CObjectList::objectChoosed, inpad,         [=]{inpad->reset();});
    connect(inpad,              &CFastDialog::canselClicked, inpad,         [=]{inpad->reset();});
    connect(ContentWidget->dnd, &CDND::launch,               inpad,         [=]{inpad->reset();});
    connect(inpad,              &CInpad::applyClicked,       inpad,         [=]{inpad->application(ObjectList->targetName, ObjectList->TypeTarget);});
    connect(inpad,              &CInpad::applyClicked,       ContentWidget, [=]{ContentWidget->updateList(target, ContentWidget->targetType);});
}

void Window::NewObjectDialog() {
    if (CConfigs::CONFIG_GAME != "None") {
        newObjectDialog = new CNewObjectDialog();
        newObjectDialog->show();
        connect(newObjectDialog, &CFastDialog::applyClicked, ObjectList, [=]{ObjectList->newObject(newObjectDialog);});
    }
    else
        ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R37);
}

void Window::settings() {
    CSettings* set = new CSettings;
}

void Window::updatePointer(CObjectsButton* pointer) {
    target = pointer;
}

void Window::inpadShow () {
    if (target != nullptr) inpad->show();
    else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R35);
}
