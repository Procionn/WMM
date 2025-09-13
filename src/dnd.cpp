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

#include "core.h"
#include "patterns/ERRORdialog.h"
#include "methods.h"
#include "ModManager.h"

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
    if (Core::CONFIG_GAME != "None") {
        try {
            for (const QUrl &url : e->mimeData()->urls()) {
                std::string fileName = url.toLocalFile().toStdString();
                stc::cerr("Dropped file:" + fileName);
                ModManager::get().load(fileName);
            }
            emit launch();
        }
        catch (const std::exception& e) {
            ERRORdialog* dialog = new ERRORdialog(std::string("DND ERROR: ") + e.what());
        }
        catch (const std::string& e) {
            ERRORdialog* dialog = new ERRORdialog(std::string("DND ERROR: ") + e);
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
}
