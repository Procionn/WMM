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

class ModObject;

class ModVersionObject final : public CBaseSmartObject
{
    Q_OBJECT
    ModObject* parent;
    static const inline std::string untoggledColor2 = "#3b3b3b";

public:
    ModVersionObject(const QString& version, ModObject* parentWidget);
    void DELETE() override;

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

private slots:
    void child_status();
    void RMB(const QPoint&, ModVersionObject*) override;
    void deletionSignals();

signals:
    void allOn();
    void noAllOn();
};

#endif // MODVERSIONLIST_H
