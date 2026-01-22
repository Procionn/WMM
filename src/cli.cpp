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
#include "cli.h"
#include "core.h"
#include "CONSTANTS.h"
#include "methods.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <filesystem>
#include <wmml.h>
#include <iostream>

namespace cli {

bool parser () {
    QCommandLineParser parser;
    parser.setApplicationDescription("Wirus Mod Manager");

    for (auto target : FlagsList) {
        QCommandLineOption clearCache(QString::fromStdString(target.first),
                                      QString::fromStdString(Core::get().lang.at(target.second)));
        parser.addOption(clearCache);
    }

    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(*QCoreApplication::instance());

    for (auto& entry : parser.optionNames()) {
        for (auto flags : cli::FlagsList) {
            if (entry.toStdString() == flags.first)
                goto cli;
        }
    }
    return false; // start gui
    cli:
    std::string flag;
    for (auto& entry : parser.optionNames()) {
        flag = entry.toStdString();
        if (flag == FlagsList[0].first)
            clear_cache();
        if (flag == FlagsList[1].first)
            clear_data();
        if (flag == FlagsList[2].first)
            migrate_data();
    }
    return true;
}


void clear_cache() {
    stc::fs::remove_all("logs");
    stc::fs::remove_all(RAM);
    stc::fs::remove_all(COLLECTIONS);
}


void clear_data() {
    clear_cache();
    stc::fs::remove_all(MODS);
    stc::fs::remove_all(GAME);
    stc::fs::remove_all(ARCHIVE);
}


void migrate_data() {
    v97_to_v98();

    std::cout << "Migration is successful!" << std::endl;
}



void v97_to_v98() {
    std::filesystem::path fname, sname;
    std::vector<std::vector<wmml::variant>> ram;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(RAM)) {
        if (entry.is_directory())
            continue;
        fname = entry.path();
        sname = std::filesystem::path(entry.path())+="2";

        wmml file(fname);
        if (file.width() != GRID_WIDTH) {
            ram.reserve(file.height());
            std::vector<wmml::variant> v(file.width());
            while(file.read(v))
                ram.emplace_back(v);

            wmml newFile(sname, GRID_WIDTH);
            for (auto t : ram) {
                t.emplace_back(((signed char)(0)));
                newFile.write(t);
            }
            std::filesystem::remove(fname);
            std::filesystem::rename(sname, fname);
        }
        ram.clear();
    }
}


} // cli namespace
