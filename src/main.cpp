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
#include "CONSTANTS.h"
#include "window.h"
#include "core.h"
#include "ModManager.h"

#include <QApplication>


int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Wirus Mod Manager");
    app.setStyle("Fusion");
    int code = 0;

    try {
        do {
            Core::get();
            ModManager::get();
            Window w;
            code = app.exec();
        } while (code == RESET);
    } catch(const char* error) {
        stc::cerr(error);
        FatalError* error_dialog = new FatalError(error, true);
        stc::cerr(error);
        error_dialog->exec();
    } catch (const std::exception& e) {
        stc::cerr(std::string("Error: ") + e.what());
        FatalError* error_dialog = new FatalError(e.what(), true);
        stc::cerr(e.what());
        error_dialog->exec();
    }
    return code;
}
