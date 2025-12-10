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
#ifndef CSMARTOBJECT_H
#define CSMARTOBJECT_H

#include <QFrame>

class CSmartObject : public QFrame
{
    Q_OBJECT

public:
    bool toggl_condition = false;

public:
    CSmartObject(QWidget *parent = nullptr);
    void ChangeOfState();

// ////////////////////////////////////////////// You can override these 3 methods if you need to.
    virtual void context(const QPoint& pos);   // The "context" method, in my case, was used to open
    virtual void turnOff();                    // the context menu when pressing the RMB
    virtual void turnOn();                     //
// //////////////////////////////////////////////

protected:
    void mousePressEvent (QMouseEvent* event);

signals:
    void clicked(QMouseEvent* event, CSmartObject* target);
    void rightClick(const QPoint& pos, CSmartObject* target);
};

#endif // CSMARTOBJECT_H
