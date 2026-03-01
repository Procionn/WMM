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
#pragma once
#include "../patterns/CFastDialog.h"
#include <string>
#include <tuple>

class CCortegeWindow : public CFastDialog
{
    class CLabel *message, *mainName, *newName;
    class QLineEdit* verLine;
    class QLineEdit* crtLine;
    class CLabel* error;
    class QGridLayout* lay;
    class QComboBox* versions;
    std::string* baseVersion;
    std::string* returnedVersion;
    std::string* returnedCortege;
    uint64_t id;

    CCortegeWindow(std::string*, std::string*, std::string*, const uint64_t modId);

    void crt(const std::string& version, const std::string& name, const bool issue = true);
    void que(const std::string& version, const std::string& name);
    void set_version(const std::string& version);
    void set_items();

public:
    static std::tuple<std::string, std::string, std::string>
    create(const std::string& version, const std::string& name, const uint64_t modId);
    static std::tuple<std::string, std::string, std::string>
    question(const std::string& version, const std::string& name, const uint64_t modId);
};
