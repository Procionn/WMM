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

bool CBaseConfig::configRead (std::ifstream& input, std::string& firstReturned, std::string& lastReturned) {
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
        config.open(CONFIG, std::ios::app);
        config_reader();
    }
    else {
        config.open(CONFIG);
        config << "WMM_CONFIG_LANGUAGES=lang/EN.ini" << "\n";
        config << "WMM_CONFIG_GAME=None" << "\n";
#ifdef WIN64
        config << "WMM_CONFIG_USE_EXTERNAL_MODULE=true" << "\n";
#else
        config << "WMM_CONFIG_USE_EXTERNAL_MODULE=false" << "\n";
#endif
        config.flush();
        config_reader();
    }
}



void CConfigs::config_reader () {
    std::ifstream file(CONFIG);
    std::string parameter, indicator;
    while (configRead(file, parameter, indicator))
        configs[parameter] = indicator;
    file.close();
}



void CConfigs::overwriting_config_data () {
    config.close();
    config.open(CONFIG, std::ios::out);
    for (const auto& entry : configs)
        config << entry.first << "=" << entry.second << "\n";
    config.flush();
}



void CConfigs::set_default (const std::string& key, const std::string& value) {
    if (configs.find(key) == configs.end()) {
        config << key << "=" << value << "\n";
        config.flush();
        configs[key] = value;
    }
}
