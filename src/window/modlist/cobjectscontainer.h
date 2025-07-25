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
#ifndef COBJECTSCONTAINER_H
#define COBJECTSCONTAINER_H

#include "cobject.h"
#include "../../patterns/CSmartList.h"

#include <QWidget>

class CObjectsContainer final : public CSmartList<CObject>
{
    Q_OBJECT
public:
    CObjectsContainer();
public slots:
    void delete_target(CObject*);

private slots:
    void deletionSignals();
    void RMB(const QPoint& pos, CObject* target) override;

signals:
    void removed(CObject*);
};

#endif // COBJECTSCONTAINER_H
