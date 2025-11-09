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
#ifndef SETSOURCE_H
#define SETSOURCE_H

#include "../patterns/CScrollWindow.h"
#include "../patterns/ERRORdialog.h"
#include "../patterns/CLinkTumbler.h"
#include "../CONSTANTS.h"
#include "../core.h"

#include <QWidget>

class setsource : public QWidget
{
    Q_OBJECT

    bool copyMode;
    QVBoxLayout* firstlist;
    QVBoxLayout* lastlist;
    QString buffer;
    CLinkTumbler* target = nullptr;

public:
    setsource();

private slots:
    void chooseGame(QPushButton* parent);
    void chooseExe(QPushButton* dirBTN);
    void createBackup();
};

#endif // SETSOURCE_H
