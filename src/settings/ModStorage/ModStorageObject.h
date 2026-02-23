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
#ifndef MODSTORAGEOBJECT_H
#define MODSTORAGEOBJECT_H

#include "../../patterns/CBaseSmartObject.h"
#include <stdint.h>

class Mod;
class ModVersionList;
class ModVersionObject;

class ModObject final : public CBaseSmartObject
{
    Q_OBJECT
    QWidget* main;
    QWidget* versions;
    const Mod* data;
    ModVersionList* list = nullptr;
    static const inline std::string untoggledColor2 = "#2e2e2e;";

public:
    ModObject(const Mod*);
    virtual void turnOff() override;
    virtual void turnOn() override;
    virtual void DELETE() override;
    std::string get_name();
    uint64_t get_mod_id();

private:
    void INFO() override;

public slots:
    void child_turnOff();
    void none_triggered_on();
    void none_triggered_off();
    void delete_child(ModVersionObject*);

signals:
    void fromChildSwitched(ModObject*);
    void remove(ModObject*);
    void childs_set_off();
    void childs_set_on();
};


#endif // MODSTORAGEOBJECT_H
