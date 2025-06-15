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
#ifndef CINPADBUTTON_H
#define CINPADBUTTON_H

#include "../../patterns/CSmartObject.h"

#include <QLabel>
#include <QVBoxLayout>

class CInpadButton final : public CSmartObject
{
    std::string name;
    QLabel* label;
public:
    bool type;
    CInpadButton(const std::string& setName, const bool& type, bool& counter);
    void turnOff() override;
    void turnOn() override;
    bool is_target();
    std::string get_name();
protected:
    std::string untoggledColor1 = " background-color: #2e2e2e;";
    std::string untoggledColor2 = " background-color: #404040;";
    std::string toggledColor1   = " background-color: #444b52;";
    std::string toggledColor2   = " background-color: #535b63;";
    bool count_type;
};

#endif // CINPADBUTTON_H
