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
#ifndef CNEWOBJECTDIALOG_H
#define CNEWOBJECTDIALOG_H

#include "../patterns/CFastDialog.h"

class QLineEdit;
class CNewObjectDialog : public CFastDialog
{
    Q_OBJECT
    QLineEdit* nameTab;

public:
    CNewObjectDialog();

protected slots:
    void test();

signals:
    void success(std::string);
};

#endif // CNEWOBJECTDIALOG_H
