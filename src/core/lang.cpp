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

Lang::Lang () {
    update_lang();
}


void Lang::update_lang () {
    if (!lang.empty())
        lang.clear();
    std::ifstream readedFile (CConfigs::CONFIG_LANGUAGES);
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator))
        lang[parameter] = indicator;
}


void Lang::load_lang_pack(const std::string& pathToFile) {
    std::ifstream readedFile (pathToFile);
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator))
        lang[parameter] = indicator;
}
