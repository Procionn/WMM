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
#include "ModStorage.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>

#define SETTING_SPAGES() \
    X(sorce,       setsource,      settings_source,      "LANG_BUTTON_SORCE")           \
    X(lang,        setlang,        settings_lang,        "LANG_BUTTON_LANG")            \
    X(support,     setsupport,     settings_support,     "LANG_BUTTON_SUPPORT")         \
    X(extensions,  setextensions,  settings_extension,   "LANG_BUTTON_EXTENSION")       \
    X(collections, collections,    settings_collections, "LANG_BUTTON_IMPORT/EXPORT")

class SObjects : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout* list;

#define X(button, class_, object, lang_str) \
    CLinkTumbler* button;
    SETTING_SPAGES()
#undef X
    
    SObjects();
};





class SList : public QWidget
{
    Q_OBJECT
    std::vector<QWidget*> pages;

public:  
    SList();

#define X(button, class_, object, lang_str) \
    class_* object;
    SETTING_SPAGES()
#undef X

public slots:
    void show(QWidget*);
};





class CSettings : public QDialog
{
    Q_OBJECT
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
