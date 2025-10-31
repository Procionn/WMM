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
#ifndef CLINKTUMBLER_H
#define CLINKTUMBLER_H

#include "CToggledButton.h"

class CLinkTumbler : public CToggledButton
{
    Q_OBJECT
    CLinkTumbler* link = nullptr;

public:
    CLinkTumbler(const std::string& name = "", CLinkTumbler* linked = nullptr);

protected slots:
    void ChangeOfState();
    void reset();
};


#endif // CLINKTUMBLER_H
