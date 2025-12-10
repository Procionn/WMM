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
#ifndef CBASESMARTOBJECT_H
#define CBASESMARTOBJECT_H

#include "CSmartObject.h"

class CBaseSmartObject : public CSmartObject
{
protected:
    CBaseSmartObject(const bool type, QWidget* parent = nullptr);
    virtual void DELETE() = 0;
    virtual void INFO() = 0;

    bool count_type = true;
    bool infoEnabled = true;
    static const inline std::string dataString = " background-color: ";
    static const inline std::string untoggledColor2 = "#2e2e2e;";
    static const inline std::string untoggledColor1 = "#404040;";
    static const inline std::string toggledColor2   = "#3f464d;";
    static const inline std::string toggledColor1   = "#535b63;";

public:
    bool get_style();
    void set_style(const bool);

    virtual void context(const QPoint& pos) override;
    virtual void turnOff() override;
    virtual void turnOn() override;
};

#endif // CBASESMARTOBJECT_H
