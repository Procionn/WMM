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
#ifndef COBJECT_H
#define COBJECT_H

#include "../../core.h"
#include "../../methods.h"
#include "../../patterns/CSwitchButton.h"
#include "../../patterns/CSmartObject.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QSplitter>
#include <QMouseEvent>
#include <string>

class CSplitter;

class CObject : public CSmartObject
{
    Q_OBJECT

public:
    std::string name;
    std::string version;
    bool type;
    uint64_t id;
    uint64_t index;

protected:
    std::string untoggledColor2 = " background-color: #2e2e2e;";
    std::string untoggledColor1 = " background-color: #404040;";
    std::string toggledColor2   = " background-color: #3f464d;";
    std::string toggledColor1   = " background-color: #535b63;";
    // std::string border          = " border-radius: 10px;";
    std::string border          = "";
    bool count_type;

public:
    QHBoxLayout* Box;
    QLabel* Lname;
    QLabel* Lversion;
    QLabel* Ltype;
    CSplitter* spl1;
    CSplitter* spl2;
    CSwitchButton* switcher;

public:
    CObject(const void* v, bool& counter, const uint64_t& index);
    void set_style(const bool type);
    bool get_style();

signals:
    void ON(CObject* pointer);
    void OFF(CObject* pointer);
    void remove(CObject* pointer);

public slots:
    void turnOff() override;
    void turnOn() override;
    void context(const QPoint& pos) override;
    void DELETE();

private slots:
    void INFO();
};





class CSplitter : public QSplitter
{
    Q_OBJECT
public:
    CSplitter(QHBoxLayout* parent);
public slots:
    void moveSplitter(int pos, int index);
};

#endif // COBJECT_H
