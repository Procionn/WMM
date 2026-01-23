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
#ifndef CLI_H
#define CLI_H
#include <vector>
#include <utility>
#include <string>



namespace cli {
    inline std::vector<std::pair<std::string, std::string>> FlagsList {
        {"clear-cache",   "CLI_CLEAR_CACHE"},
        {"clear-data",    "CLI_CLEAR_DATA"},
        {"migrate",       "CLI_MIGRATE_DATA"},
        {"re-gen-config", "CLI_GENERATE_CONFIG"}
        // {"update",        "CLI_APP_UPDATE"}
    };

    bool parser();
    // void app_apdate();
    void clear_cache();
    void clear_data();
    void re_gen_config();

    void migrate_data();
    void v97_to_v98();
}

#endif // CLI_H
