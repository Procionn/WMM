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
#include "mainWidgets.h"

#include "../window/modlist/cobject.h"
#include <QLabel>
#include <wmml.h>
#include "../settings/settings.h"

CContentList::CContentList () {
    setMinimumWidth(200);
    filter = CObjectsContainer::NAME_T;
    QVBoxLayout* BaseContainer = new QVBoxLayout();
    setLayout(BaseContainer);
    siFrame = new CSubInfoFrame(BaseContainer);
    contentList = new CObjectsContainer;
    searchWidget = new CSearchWidget;
    BaseContainer->addWidget(contentList);
    BaseContainer->addWidget(searchWidget);
    BaseContainer->setSpacing(0);
    dnd = new CDND(BaseContainer, Core::lang["LANG_LABEL_DND"]);
    connect(siFrame, &CSubInfoFrame::filter_changed, this, &CContentList::sort);
    connect(searchWidget, &CSearchWidget::search_updated, contentList, &CObjectsContainer::search);
    connect(dnd, &CDND::launch, []{CSettings::get()->settings_modules_list->settings_storage->update();});
}

CContentList::~CContentList() {
    delete file;
}


void CContentList::updateList (CObjectsButton* pointer, bool type) {
    targetName = pointer->name;
    targetType = type;
    clear();
    if (type) sPath = stc::cwmm::ram_preset(targetName);
    else      sPath = stc::cwmm::ram_collection(targetName);
    delete file;
    file = new wmml(sPath);
    std::vector<wmml::variant> v(GRID_WIDTH);
    bool counter = false;
    for (uint64_t index = 0; file->read(v); ++index) {
        CObject* buttonWidget = new CObject(static_cast<void*>(&v), counter, index);
        contentList->add(buttonWidget);
        buttonWidget->spl1->setSizes(siFrame->spl1->sizes());
        buttonWidget->spl2->setSizes(siFrame->spl2->sizes());
        buttonWidget->show();

        connect(siFrame->spl1,  &QSplitter::splitterMoved, buttonWidget->spl1, &CSplitter::moveSplitter);
        connect(siFrame->spl2,  &QSplitter::splitterMoved, buttonWidget->spl2, &CSplitter::moveSplitter);
        connect(buttonWidget,   &CObject::ON,     this, &CContentList::changeStatusOn);
        connect(buttonWidget,   &CObject::OFF,    this, &CContentList::changeStatusOff);
        connect(buttonWidget,   &CObject::priority_changed, this, &CContentList::priority_changing);
        connect(buttonWidget,   &CObject::flushing_request, this, &CContentList::flush);
        connect(contentList,    &CObjectsContainer::flushing_request, this, &CContentList::flush);
        connect(buttonWidget,   &CObject::remove, contentList, &CObjectsContainer::delete_target);
        connect(contentList,    &CObjectsContainer::removed, this, &CContentList::deleting);
        connect(buttonWidget->Lversion, &CVersion::version_changed, this, &CContentList::change_version);
    }
    sort();
}

void CContentList::change_version (const std::string_view& version, const uint64_t index) {
    file->overwriting_sector(index, 1, std::string(version));
    flush();
}

void CContentList::clear () {
    contentList->clear();
}

void CContentList::changeStatusOn(CObject* toggledElements) {
    file->overwriting_sector(toggledElements->index, 4, true);
    flush();
}
void CContentList::changeStatusOff(CObject* toggledElements) {
    file->overwriting_sector(toggledElements->index, 4, false);
    flush();
}
void CContentList::deleting (CObject* pointer) {
    static CObject* last;
    if (last != pointer) {
        file->remove_object(pointer->index);
        flush();
        delete pointer;
        last = pointer;
    }
}

void CContentList::sort (const int filter) {
    if (filter == INT_MAX)
        contentList->sort(this->filter);
    else {
        contentList->sort(filter);
        this->filter = filter;
    }
}

void CContentList::show_search_widget () {
    searchWidget->show();
}

void CContentList::hide_search_widget () {
    searchWidget->hide();
}

void CContentList::priority_changing (CObject* toggledElements, signed char priority) {
    file->overwriting_sector(toggledElements->index, 5, priority);
}

void CContentList::flush() {
    file->flush();
}

