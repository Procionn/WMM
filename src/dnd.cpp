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
#include "dnd.h"

#include "CONSTANTS.h"
#include "core.h"
#include "patterns/ERRORdialog.h"
#include "methods.h"
#include "ModManager.h"

#include <iostream>
#include <hpp-archive.h>
#include <QDragEnterEvent>
#include <QMimeData>



CDND::CDND(QVBoxLayout* parent, std::string name) {
    parent->addWidget(this);
    this->setText(QString::fromStdString(name));
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setMinimumHeight(70);
    setMaximumHeight(100);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setStyleSheet("background-color: #444b50; border-radius: 10px;");
}


void CDND::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void CDND::dropEvent(QDropEvent* e) {
    if (CConfigs::CONFIG_GAME != "None") {
        try {
            for (const QUrl &url : e->mimeData()->urls()) {
                std::string fileName = url.toLocalFile().toStdString();
                stc::cerr("Dropped file:" + fileName);
#if 0
                regex(fileName);
                std::filesystem::path modsDir = stc::cwmm::ram_mods(name);
                if (std::filesystem::exists(modsDir))
                    stc::fs::remove_all(modsDir);
                modInfoSave(modsDir);

                ArchiveReader archive(fileName);
                archive.set_export_directory(modsDir);
                for (const auto* entry : archive)
                    archive.write_on_disk();


                std::filesystem::path path = ARCHIVE + Core::CONFIG_GAME + "/" + id;
                std::filesystem::create_directories(path);
                std::ofstream modStructFile(path / (version + EXPANSION2));
                for (const auto& entry : std::filesystem::recursive_directory_iterator(modsDir)) {
                    if (std::filesystem::is_regular_file(entry.path())) {
                        std::filesystem::path relative = std::filesystem::relative(entry.path(), modsDir);
                        modStructFile << relative.generic_string() << "\n";
                    }
                }
                modStructFile.close();

                stc::cerr("The archive has been successfully extracted to the folder: " + modsDir.string());
#else
                ModManager::get().load(fileName);
#endif
            }
            emit launch();
        }
        catch (const std::exception& e) {
            ERRORdialog* dialog = new ERRORdialog(std::string("DND ERROR: ") + e.what());
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
}


void CDND::modInfoSave(const std::filesystem::path& directory) {
#if 0
    std::filesystem::path path = directory / (WML + name + EXPANSION2);
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path);
    file << name << "\n";
    file << version << "\n";
    file << id << "\n";
    file.close();
#endif
}
