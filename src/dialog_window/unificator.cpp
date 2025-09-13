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
#include <archive.h>
#include <QCloseEvent>

std::string unificator::start (void* versionList, const std::string& name,
                              const std::string& modVersion, const uint64_t& id) {
    unificator* window = new unificator(versionList, name, modVersion, id);
    std::string version;
    window->secondPage->string = &version;
    window->string = &version;
    window->exec();
    return version;
}


unificator::unificator(void* versionList, const std::string& name,
                       const std::string& version, const uint64_t& id) : modId(id) {
    QVBoxLayout* baseLay = new QVBoxLayout(this);
    QHBoxLayout* buttonBox = new QHBoxLayout();
    QLabel* frame = new QLabel;
    back = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_BACK"]));
    next = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_NEXT"]));
    newVersion = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_NEW_VERSION"]));
    firstPage = new questioner(name, newVersion);
    secondPage = new choicer(versionList);
    thirdPage = new renamer(version);

    baseLay->addWidget(firstPage);
    baseLay->addWidget(secondPage);
    baseLay->addWidget(thirdPage);
    baseLay->addWidget(frame);
    baseLay->addLayout(buttonBox);
    buttonBox->addWidget(back);
    buttonBox->addWidget(next);

    setMinimumHeight(300);
    setMinimumWidth(450);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    next->setDefault(true);
    back->setDown(true);

    connect(back,       &QPushButton::clicked, this, &unificator::back_click);
    connect(next,       &QPushButton::clicked, this, &unificator::next_click);
    connect(newVersion, &QPushButton::clicked, this, &unificator::version_click);
}


void unificator::back_click () {
    switch (status) {
    case 1:
    case 2:
        status = 0;
        firstPage->show();
        secondPage->hide();
        thirdPage->hide();
        break;
    case 0:
        string->clear();
        delete this;
        break;
    }
}


void unificator::next_click () {
    switch (status) {
    case 0:
        status = 1;
        firstPage->hide();
        secondPage->show();
        thirdPage->hide();
        break;
    case 1:
        if (!string->empty())
            delete this;
        break;
    case 2:
        if (ModManager::get().exists(modId, thirdPage->string->text().toStdString())) {
            thirdPage->errorLabel->show();
            return;
        }
        else {
            string->append(thirdPage->string->text().toStdString());
            delete this;
        }
        break;
    }
}


void unificator::closeEvent (QCloseEvent* e) {
    string->clear();
    delete this;
}


void unificator::reject () {
    string->clear();
    delete this;
}


void unificator::version_click () {
    status = 2;
    firstPage->hide();
    secondPage->hide();
    thirdPage->show();
}


questioner::questioner (const std::string& name, QPushButton* btn) {
    QVBoxLayout* lay = new QVBoxLayout(this);
    QLabel* label = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_UNIFICATE"]));
    QWidget* widget = new QWidget;
    QVBoxLayout* buttonLay = new QVBoxLayout(widget);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    label->setWordWrap(true);
    lay->setAlignment(Qt::AlignTop);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    buttonLay->setAlignment(Qt::AlignBottom);
    lay->addWidget(label);
    lay->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_NAME"] + " " + name)));
    buttonLay->addWidget(btn);
    lay->addWidget(widget);
    show();
}



choicer::choicer (void* versionList) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    QVBoxLayout* list = new QVBoxLayout(this);
    list->setAlignment(Qt::AlignTop);

    auto* vector = static_cast<std::vector<ModInfo>*>(versionList);
    CLinkTumbler* lastButtonPointer = nullptr;
    for (auto ptr = vector->begin(), end = vector->end(); ptr != end; ++ptr) {
        auto* button = new CLinkTumbler(ptr->modVersion, lastButtonPointer);
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, this, &choicer::buttonClick);
    }
    hide();
}


void choicer::buttonClick (CToggledButton*, const std::string& name) {
    string->assign(name);
}



renamer::renamer (const std::string& version) {
    QVBoxLayout* list = new QVBoxLayout(this);
    string = new QLineEdit(this);
    errorLabel = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_EXISTS"]));
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    list->setAlignment(Qt::AlignTop);
    string->setText(QString::fromStdString(version));
    list->addWidget(string);
    list->addWidget(errorLabel);
    errorLabel->hide();
    hide();
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
