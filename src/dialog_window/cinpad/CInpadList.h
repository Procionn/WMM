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
#ifndef CINPADLIST_H
#define CINPADLIST_H

#include "../../patterns/CSmartList.h"
#include "CInpadButton.h"

class CInpadList final : public CSmartList<CInpadButton>
{
    Q_OBJECT
public:
    CInpadList();
private slots:
    void controller (QMouseEvent* event, CSmartObject* target) override;
};

#endif // CINPADLIST_H
