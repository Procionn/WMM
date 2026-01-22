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
#ifndef TEMPLATELIST_H
#define TEMPLATELIST_H

#include <QVBoxLayout>
#include <QScrollArea>

#include "CSmartObject.h"

class TemplateList : public QScrollArea
{
    Q_OBJECT

protected:
    QVBoxLayout* list;

public:
    TemplateList(QWidget* parent = nullptr);

protected:
    void connecting(CSmartObject* linkable);
    void updated();
    void reseted();

    virtual void controller(QMouseEvent* event, CSmartObject* target) = 0;
    virtual void rightClickController(const QPoint& pos, CSmartObject* target) = 0;

signals:
    void update();
    void reseting();
};

#endif // TEMPLATELIST_H
