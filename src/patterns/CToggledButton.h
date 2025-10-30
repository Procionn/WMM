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
#ifndef CTOGGLEDBUTTON_H
#define CTOGGLEDBUTTON_H

#include <QPushButton>
#include <string>

class CToggledButton : public QPushButton
{
    Q_OBJECT

public:
    const std::string name;
    bool selected;

    CToggledButton(const std::string& setName = "", bool type = false);
    void SetLeftAlignment(bool type);
    void setTheme(const std::string& type);
    void setColor(const std::string& toggled, const std::string& untoggled);
    void isTarget(bool type);

private:
    static const inline std::string TBC1 = "=> ";
    static const inline std::string TBC2 = " <=";

protected:
    bool leftAlignment = false;
    static const inline std::string leftAlign = "text-align: left; padding-left: 10px;";
    std::string untoggledColor = " background-color: #31363b;";
    std::string toggledColor = " background-color: #444b52;";

protected slots:
    virtual void ChangeOfState();

signals:
    void toggled(CToggledButton* toggledButton, std::string name);
    void untoggled(CToggledButton* toggledButton, std::string name);
};

#endif // CTOGGLEDBUTTON_H
