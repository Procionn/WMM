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
#include "../core.h"

#include "../CONSTANTS.h"

bool CBaseConfig::configRead (std::istream& input, std::string& firstReturned, std::string& lastReturned) {
    std::string readedLine;
    if (std::getline(input, readedLine)) {
        size_t part = readedLine.find_first_of('=');
        firstReturned = readedLine.substr(0, part);
        lastReturned = readedLine.substr(part + 1);
        if (lastReturned.back() == '\r')
            lastReturned.resize(lastReturned.size() - 1);
        return true;
    }
    else return false;
}





CConfigs::CConfigs () {
    if (std::filesystem::exists(CONFIG)) {
        config_reader();
    }
    else {
        std::ofstream config(CONFIG, std::ios::app);
        config << "Languages=lang/EN.ini" << "\n";
        config << "Game=None" << "\n";
        config.close();
        config_reader();
    }
}



void CConfigs::config_reader () {
    std::ifstream readedFile(CONFIG);
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
        if (parameter == "Languages") CONFIG_LANGUAGES = indicator;
        else if (parameter == "Game") CONFIG_GAME      = indicator;
    }
    readedFile.close();
}



void CConfigs::overwriting_config_data () {
    std::ofstream file(CONFIG, std::ios::out);
    file << "Languages=" << CONFIG_LANGUAGES << "\n";
    file << "Game=" << CONFIG_GAME << "\n";
    file.close();
}
