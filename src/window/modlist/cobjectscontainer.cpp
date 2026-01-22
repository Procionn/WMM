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
#include "cobjectscontainer.h"
#include "../../dialog_window/CProperties.h"
#include <regex>

CObjectsContainer::CObjectsContainer() {
    list = new QVBoxLayout;
    QWidget* scrolledWidget = new QWidget;

    list->setAlignment(Qt::AlignTop);
    setWidgetResizable(true);
    setWidget(scrolledWidget);
    scrolledWidget->setLayout(list);
    list->setSpacing(0);
}


void CObjectsContainer::RMB (const QPoint& pos, CObject* target) {
    QMenu* contextMenu = new QMenu(this);
    QAction* action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &CObjectsContainer::deletionSignals);
    QAction* action3 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_PROPERTIES"]));
    connect(action3, &QAction::triggered, this, &CObjectsContainer::change_priority_window);
    contextMenu->exec(QCursor::pos());
}

void CObjectsContainer::change_priority_window() {
    CProperties* window = new CProperties(0);
    connect(window, &CProperties::complited, [this](signed char newValue){
        change_priority(newValue);
        emit flushing_request();
    });
}

void CObjectsContainer::change_priority(signed char newValue) {
    for (CObject* target : childList) {
        if (target->is_target())
            target->PRIORITY(newValue);
    }
}

void CObjectsContainer::deletionSignals () {
    std::vector<CObject*> newVector;
    newVector.reserve(childList.size());
    for (CObject* target : childList) {
        if (target->is_target())
            target->DELETE();
        else newVector.emplace_back(target);
    }
    childList.clear();
    childList = std::move(newVector);
}


void CObjectsContainer::delete_target(CObject* target) {
    auto iterator = std::find(childList.begin(), childList.end(), target);
    if (iterator == childList.end())
        std::runtime_error(std::string("the object ") + target->name + " was not found");
    childList.erase(iterator);
    emit removed(target);
}


#define sorting(symbol, comp_type)  \
std::sort(childList.begin(), childList.end(), [](CObject* a, CObject* b) {  \
    return a->comp_type symbol b->comp_type;    \
})

void CObjectsContainer::sort (const int filter) {
    switch (filter) {
    case NAME_T:
        sorting(>, name);
        break;
    case NAME_F:
        sorting(<, name);
        break;
    case VERSION_T:
        sorting(>, version);
        break;
    case VERSION_F:
        sorting(<, version);
        break;
    case TYPE_T:
        sorting(>, type);
        break;
    case TYPE_F:
        sorting(<, type);
        break;
    case CONDITION_T:
        sorting(>, switcher->selected);
        break;
    case CONDITION_F:
        sorting(<, switcher->selected);
        break;
    default:
        stc::cerr("undefined filter");
        return;
    }
    for (QLayoutItem* item; (item = list->takeAt(0)) != nullptr; delete item)
        if (item->widget())
            list->removeWidget(item->widget());
    for (auto* entry : childList)
        list->addWidget(entry);
    coloring();
}



void CObjectsContainer::coloring () {
    CObject* last = nullptr;
    for (auto* entry : childList) {
        if (entry->isVisible()) {
            if (!last) {
                entry->set_style(true);
                last = entry;
            }
            else {
                entry->set_style(!(last->get_style()));
                last = entry;
            }
        }
    }
}



void CObjectsContainer::search (const std::string& ref) {
    for (auto* target : childList) {
        if (std::regex_search(target->name, std::regex(ref, std::regex_constants::icase)))
            target->show();
        else target->hide();
    }
    coloring();

}
