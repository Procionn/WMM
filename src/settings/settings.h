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
#ifndef SETTINGS_H
#define SETTINGS_H

#include "../patterns/CLinkTumbler.h"
#include "setlang.h"
#include "setsource.h"
#include "setsupport.h"
#include "setextensions.h"
#include "collections.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>

class SObjects : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout* list;
    CLinkTumbler* sorce;
    CLinkTumbler* lang;
    CLinkTumbler* support;
    CLinkTumbler* extensions;
    CLinkTumbler* collections;
    
    SObjects();
};





class SList : public QWidget
{
    Q_OBJECT
public:  
    SList();

    setsource* settings_source;
    setlang* settings_lang;
    setsupport* settings_support;
    setextensions* settings_extension;
    collections* settings_collections;

public slots:
    void sorce();
    void lang();
    void support();
    void extensions();
    void collection();
};





class CSettings : public QDialog
{
    Q_OBJECT
    std::vector<std::function<void()>> tasks;
    static inline CSettings* object;

public:
    CSettings();
    SObjects* sobjects;
    SList* settings_modules_list;
    
    static CSettings* get();

signals:
    void save();
};

#endif // SETTINGS_H
