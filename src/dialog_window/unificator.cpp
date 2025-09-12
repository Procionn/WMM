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
#include "unificator.h"

#include "../core.h"
#include "../ModManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <archive.h>

std::string unificator::start (void* versionList) {
    unificator* window = new unificator(versionList);
    std::string version;
    window->secondPage->string = &version;
    window->string = &version;
    window->exec();
    return version;
}


unificator::unificator(void* versionList) {
    QVBoxLayout* baseLay = new QVBoxLayout(this);
    QHBoxLayout* buttonBox = new QHBoxLayout();
    QLabel* frame = new QLabel;
    back = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_BACK"]));
    next = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_NEXT"]));
    firstPage = new questioner();
    secondPage = new choicer(versionList);

    baseLay->addWidget(firstPage);
    baseLay->addWidget(secondPage);
    baseLay->addWidget(frame);
    baseLay->addLayout(buttonBox);
    buttonBox->addWidget(back);
    buttonBox->addWidget(next);

    setMinimumHeight(300);
    setMinimumWidth(450);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    connect(back, &QPushButton::clicked, this, &unificator::back_click);
    connect(next, &QPushButton::clicked, this, &unificator::next_click);
}


void unificator::back_click () {
    if (status) {
        status = false;
        firstPage->show();
        secondPage->hide();
    }
    else {
        string->clear();
        delete this;
    }
}


void unificator::next_click () {
    if (status)
        delete this;
    else {
        status = true;
        firstPage->hide();
        secondPage->show();
    }
}


questioner::questioner () {
    QVBoxLayout* lay = new QVBoxLayout(this);
    QLabel* label = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_UNIFICATE"]));
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    label->setWordWrap(true);
    lay->addWidget(label);
    lay->setAlignment(Qt::AlignTop);
    show();
}



choicer::choicer (void* versionList) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    hide();
    QVBoxLayout* list = new QVBoxLayout(this);
    list->setAlignment(Qt::AlignTop);

    auto* vector = static_cast<std::vector<ModInfo>*>(versionList);
    CLinkTumbler* lastButtonPointer = nullptr;
    for (auto ptr = vector->begin(), end = vector->end(); ptr != end; ++ptr) {
        auto* button = new CLinkTumbler(ptr->modVersion, lastButtonPointer);
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, this, &choicer::buttonClick);
    }
}


void choicer::buttonClick (CToggledButton*, const std::string& name) {
    string->assign(name);
}





CustomArchive::CustomArchive (const std::filesystem::path& path, const std::filesystem::path& exp)
    : ArchiveWriter(path, exp) {
    error_handler(archive_write_close(main));
    error_handler(archive_write_free(main));
    open();
}

void CustomArchive::open () {
    main = archive_write_new();
    archive_write_set_format_zip(main);

#ifdef __linux__
    archive_write_open_filename(main, (path += expansion).u8string().c_str());
#elif WIN32
    archive_write_open_filename_w(main, (path += expansion).wstring().c_str());
#endif
}
