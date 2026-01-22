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
#ifndef MODVERSIONLIST_H
#define MODVERSIONLIST_H

#include "../../patterns/CSmartList.h"
#include "../../patterns/CBaseSmartObject.h"
#include <iostream>
class ModObject;

class ModVersionObject final : public CBaseSmartObject
{
    Q_OBJECT
    static const inline std::string untoggledColor2 = "#3b3b3b";
    const std::string_view name;

public:
    ModObject* parent;

    ModVersionObject(const std::string_view version, ModObject* parentWidget);
    void DELETE() override;
    std::string_view get_name();

private:
    void INFO() override {}

signals:
    void remove(ModVersionObject*);
};



class ModVersionList final : public CSmartList<ModVersionObject>
{
    Q_OBJECT
    QSize sizeHint() const {
        return widget()->sizeHint();
    }
    QSize minimumSizeHint() const {
        return widget()->minimumSizeHint();
    }
public:
    ModVersionList(QWidget* parent = nullptr);

public slots:
    void set_on();
    void set_off();
    void delete_target(ModVersionObject* target);

private slots:
    void child_status();
    void RMB(const QPoint&, ModVersionObject*) override;
    void deletion_signals();

signals:
    void allOn();
    void noAllOn();
};

#endif // MODVERSIONLIST_H
