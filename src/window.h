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
#ifndef WINDOW_H
#define WINDOW_H

#include "dialog_window/cinpad/CInpad.h"
#include "window/mainWidgets.h"
#include "window/CMenuBar.h"
#include "settings/settings.h"

#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>


class Window : public QMainWindow
{
Q_OBJECT
    CSettings* settingsWindow = nullptr;

    void closeEvent(QCloseEvent *event) override;
public:
    CNewObjectDialog* newObjectDialog;
    CObjectList* ObjectList;
    CContentList* ContentWidget;
    CObjectsButton* target = nullptr;
    CInpad* inpad;
    
    Window();
    ~Window();
public slots:
    void NewObjectDialog();
    void settings();
    void updatePointer(CObjectsButton* pointer);
    void inpadShow();
private slots:
    void inpad_reset();
    void grounding(CObjectsButton* pointer);
};

#endif // WINDOW_H
