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
#ifndef MODSTORAGELIST_H
#define MODSTORAGELIST_H

#include "../../patterns/CSmartList.h"
#include "ModStorageObject.h"


class ModStorageList final : public CSmartList<ModObject>
{
public:
    ModStorageList();

public slots:
    void search(const QString& ref);

private slots:
    void deletion_signals();
    void last_target_update(ModObject*);
    void delete_target(ModObject*);
    void RMB(const QPoint& pos, ModObject* target) override;
};

#endif // MODSTORAGELIST_H
