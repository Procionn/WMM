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
#ifndef SETEXTENSIONS_H
#define SETEXTENSIONS_H

#include <QWidget>
#include <QLabel>
#include <vector>

class setextensions : public QWidget
{
    Q_OBJECT
    class QVBoxLayout* list;

    std::vector<QWidget*> expansionList;
    void clear_list();
    void generate_buttons();

public:
    setextensions();
    ~setextensions();
};



class PluginInterface;
class PluginIco : public QFrame
{
    class QVBoxLayout* vLay;
    class PCLabel *name, *description;
    PluginInterface* child;

public:
    PluginIco(PluginInterface*);
    ~PluginIco() = default;

protected:
    void mousePressEvent(QMouseEvent* event) override;
};



class PCLabel : public QLabel
{
    Q_OBJECT
public:
    PCLabel(const QString& str);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked(QMouseEvent* event);
};

#endif // SETEXTENSIONS_H
