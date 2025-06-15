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
#ifndef CFASTDIALOG_H
#define CFASTDIALOG_H

#include "../core.h"
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CFastDialog : public QDialog
{
    Q_OBJECT
protected:
    QHBoxLayout* DialogButtonBox;
    bool autodeleting = true;

public:
    QWidget* list;
    QPushButton* apply;

    CFastDialog();
signals:
    void applyClicked();
    void canselClicked();
};

#endif // CFASTDIALOG_H
