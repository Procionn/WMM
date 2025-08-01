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
#ifndef DND_H
#define DND_H

#include <QVBoxLayout>
#include <QLabel>

class CDND : public QLabel
{
    Q_OBJECT
public:
    CDND(QVBoxLayout* parent = nullptr, std::string name = "");
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);

signals:
    void launch();
};
#endif // DND_H
