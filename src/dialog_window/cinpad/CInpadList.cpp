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
#include "CInpadList.h"

CInpadList::CInpadList() {}


void CInpadList::controller (QMouseEvent* event, CSmartObject* target) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ControlModifier)
            LBM_CTRL(static_cast<CInpadButton*>(target));
        else if (event->modifiers() == Qt::ShiftModifier)
            LBM_SHIFT(static_cast<CInpadButton*>(target));
        else {
            target->ChangeOfState();
            lastTarget = static_cast<CInpadButton*>(target);
        }
    }
}
