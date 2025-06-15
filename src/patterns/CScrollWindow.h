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
#ifndef CSCROLLWINDOW_H
#define CSCROLLWINDOW_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

template <typename T, typename U>
void addScrollable (T* parent, U* child) {
    static_assert(std::is_base_of<QLayout, U>::value, "Child must be inherited from QLayout");
    QScrollArea* scroll = new QScrollArea;
    QWidget* scrolledWidget = new QWidget;
    scroll->setWidgetResizable(true);

    if constexpr (std::is_base_of<QWidget, T>::value) {
        QVBoxLayout* lay = new QVBoxLayout(parent);
        lay->addWidget(scroll);
    }
    else if constexpr (std::is_base_of<QLayout, T>::value) {
        parent->addWidget(scroll);
    }
    scroll->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}


#endif // CSCROLLWINDOW_H
