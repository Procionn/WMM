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
#include "RenameDialog.h"

#include "../core.h"

RenameDialog::RenameDialog(std::string& renamedFile, const std::string& messageString) {
    QVBoxLayout* layout = new QVBoxLayout(list);
    message = new QLabel(QString::fromStdString(messageString));
    line = new QLineEdit(QString::fromStdString(renamedFile));

    layout->addWidget(message);
    layout->addWidget(line);

    connect(this, &CFastDialog::applyClicked, [&]{
        renamedFile = line->text().toStdString();
        delete this;
    });

    layout->setAlignment(Qt::AlignTop);
    autodeleting = true;
    show();
}



std::string file_renaming(const std::string& renamedFile, std::string errorMessage) {
    std::string filename = renamedFile;
    if (errorMessage == "")
        errorMessage = Core::lang["LANG_LABEL_RENAME_MESSAGE"];

    RenameDialog* dialog = new RenameDialog(filename, errorMessage);
    dialog->exec();
    if (filename != renamedFile)
        return filename;
    else return "";
}
