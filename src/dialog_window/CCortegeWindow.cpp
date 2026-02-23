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
#include "CCortegeWindow.h"
#include "../ModManager.h"
#include "../core.h"
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class CLabel : public QLabel
{
public:
    CLabel (const std::string& str) : QLabel(QString::fromStdString(str)) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    }
    void setText (const std::string& str) { QLabel::setText(QString::fromStdString(str)); }
};



CCortegeWindow::CCortegeWindow (std::string* ptr, std::string* sPtr, std::string* tPtr,
                                const uint64_t modId)
    : returnedVersion(ptr), returnedCortege(sPtr), baseVersion(tPtr), id(modId) {
    lay = new QGridLayout(list);
    lay->setAlignment(Qt::AlignTop);
    connect(this, &CCortegeWindow::canselClicked, [this] {
        returnedVersion->clear();
        returnedCortege->clear();
    });
}


void CCortegeWindow::set_version (const std::string& version) {
    error = new CLabel("");
    CLabel* label = new CLabel(Core::lang["LANG_LABEL_CHOOSE_VERSION"]);
    lay->addWidget(error, 1, 0, 1, -1);
    lay->addWidget(label, 2, 0, 1, -1);
    lay->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"])), 3, 0);
    verLine = new QLineEdit(QString::fromStdString(version));
    lay->addWidget(verLine, 3, 1);
    connect(apply, &QPushButton::clicked, [this] {
        if (verLine->text().isEmpty()) {
            error->setText(Core::lang["LANG_LABEL_EMPTY"]);
            return;
        }
        if (ModManager::get().exists(id, verLine->text().toStdString())) {
            error->setText(Core::lang["LANG_LABEL_VERSION_EXISTS"]);
            return;
        }
        *returnedVersion = verLine->text().toStdString();
        returnedCortege->clear();
        this->deleteLater();
    });
}


void CCortegeWindow::set_items () {
    versions = new QComboBox();
    QStringList array;
    auto ref = ModManager::get().all_versions_list(id);
    array.reserve(ref.size());
    for (const auto& entry : ref)
        array.append(QString(entry.data()));
    versions->addItems(array);
}


void CCortegeWindow::crt (const std::string& version, const std::string& name, const bool issue) {
    error = new CLabel("");
    CLabel* label = new CLabel(Core::lang["LANG_LABEL_CREATE_CORTEGE"]);
    lay->addWidget(error, 1, 0, 1, -1);
    if (issue)
        lay->addWidget(label, 2, 0, 1, -1);

    lay->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"])), 3, 0);
    lay->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CORTEGE"])), 4, 0);
    lay->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CORTEGE"])), 5, 0);
    verLine = new QLineEdit(QString::fromStdString(version));
    crtLine = new QLineEdit(QString::fromStdString("cortege-"+name));
    set_items();
    lay->addWidget(verLine, 3, 1);
    lay->addWidget(crtLine, 4, 1);
    lay->addWidget(versions, 5, 1);
    connect(apply, &QPushButton::clicked, [this] {
        if (verLine->text().isEmpty() || crtLine->text().isEmpty()) {
            error->setText(Core::lang["LANG_LABEL_EMPTY"]);
            return;
        }
        if (ModManager::get().exists(id, crtLine->text().toStdString())) {
            error->setText(Core::lang["LANG_LABEL_CORTEGE_EXISTS"]);
            return;
        }
        if (ModManager::get().exists(id, verLine->text().toStdString())) {
            error->setText(Core::lang["LANG_LABEL_VERSION_EXISTS"]);
            return;
        }
        if (versions->currentText().isEmpty()) {
            error->setText(Core::lang["LANG_LABEL_MAIN_VERSION_NOT_SELECTED"]);
            return;
        }
        *returnedVersion = verLine->text().toStdString();
        *returnedCortege = crtLine->text().toStdString();
        *baseVersion = versions->currentText().toStdString();
        this->deleteLater();
    });
}

void CCortegeWindow::que (const std::string& version, const std::string& name) {
    CLabel* message = new CLabel(Core::lang["LANG_LABEL_CORTEGE_QUESTION"]);
    CLabel* mainName = new CLabel(ModManager::get().mod_data_converter(id));
    CLabel* newName = new CLabel(name);

    QPushButton* setVersion =
        new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_SET_VERSION"]));
    lay->addWidget(message, 1, 0, 1, -1);
    lay->addWidget(mainName, 2, 0, 1, -1);
    lay->addWidget(newName, 3, 0, 1, -1);
    DialogButtonBox->addWidget(setVersion);
    connect(
        apply, &QPushButton::clicked, this,
        [this, message, setVersion, version, name, mainName, newName] {
            delete message;
            delete setVersion;
            delete mainName;
            delete newName;
            crt(version, name, false);
        },
        Qt::SingleShotConnection);
    connect(
        setVersion, &QPushButton::clicked, this,
        [this, message, setVersion, version, mainName, newName] {
            delete message;
            delete setVersion;
            delete mainName;
            delete newName;
            set_version(version);
        },
        Qt::SingleShotConnection);
}


std::tuple<std::string, std::string, std::string>
CCortegeWindow::create (const std::string& version, const std::string& name, const uint64_t modId) {
    std::string returnedVersion, returnedCortege, baseVersion{};
    CCortegeWindow* dialog =
        new CCortegeWindow(&returnedVersion, &returnedCortege, &baseVersion, modId);
    dialog->crt(version, name);
    dialog->exec();
    return {returnedVersion, returnedCortege, baseVersion};
}

std::tuple<std::string, std::string, std::string>
CCortegeWindow::question (const std::string& version, const std::string& name,
                          const uint64_t modId) {
    std::string returnedVersion, returnedCortege, baseVersion{};
    CCortegeWindow* dialog =
        new CCortegeWindow(&returnedVersion, &returnedCortege, &baseVersion, modId);
    dialog->que(version, name);
    dialog->exec();
    return {returnedVersion, returnedCortege, baseVersion};
}
