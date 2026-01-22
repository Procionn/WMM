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
#include "../../patterns/CFastDialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QSplitter>
#include <QMouseEvent>
#include <string>

class CSplitter;
class CVersion;

class CObject final : public CSmartObject
{
    Q_OBJECT

public:
    std::string name;
    std::string version;
    bool type;
    uint64_t id;
    const uint64_t index;
    signed char priority;

protected:
    static const inline std::string untoggledColor2 = " background-color: #2e2e2e;";
    static const inline std::string untoggledColor1 = " background-color: #404040;";
    static const inline std::string toggledColor2   = " background-color: #3f464d;";
    static const inline std::string toggledColor1   = " background-color: #535b63;";
    // std::string border          = " border-radius: 10px;";
    static const inline std::string border          = "";
    bool count_type;

public:
    QHBoxLayout* Box;
    QLabel* Lname;
    CVersion* Lversion;
    QLabel* Ltype;
    CSplitter* spl1;
    CSplitter* spl2;
    CSwitchButton* switcher;

public:
    CObject(const void* v, bool& counter, const uint64_t& index);
    void set_style(const bool type);
    bool get_style();
    void set_priority(signed char newValue);
    char get_priority();

signals:
    void ON(CObject* pointer);
    void OFF(CObject* pointer);
    void remove(CObject* pointer);
    void flushing_request();
    void priority_changed(CObject*, signed char);

public slots:
    void turnOff() override;
    void turnOn() override;
    void context(const QPoint& pos) override;
    void DELETE();
    void PRIORITY(signed char);

private slots:
    void INFO();
    void local_priority();
};





class CVersion final : public QLabel
{
    Q_OBJECT
    CObject* parent;

public:
    CVersion(const std::string& version, CObject* parent);

protected slots:
    void context(const QPoint& pos);

signals:
    void version_changed(const std::string_view&, const uint64_t);
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
