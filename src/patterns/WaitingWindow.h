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
#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H

#include <QProgressDialog>
#include <QApplication>
#include <thread>
#include <atomic>
#include "../core.h"

#define Wait(func)                              \
    QProgressDialog d(QString::fromStdString(   \
    Core::lang["LANG_LABERL_WAIT"]), nullptr, 0, 100, nullptr); \
    d.setWindowModality(Qt::WindowModal);       \
    d.setMinimumDuration(0);                    \
    d.show();                                   \
    std::atomic<bool> flag = false;             \
    std::thread taskThread([this, &flag]{func; flag.store(true);});   \
    while (!flag) QApplication::processEvents();\
    taskThread.join();                          \
    d.setValue(100)


#define Wait2(func, PARAMETERS)                 \
    QProgressDialog d(QString::fromStdString(   \
    Core::lang["LANG_LABERL_WAIT"]), nullptr, 0, 100, nullptr); \
    d.setWindowModality(Qt::WindowModal);       \
    d.setMinimumDuration(0);                    \
    d.show();                                   \
    std::atomic<bool> flag = false;             \
    std::thread taskThread([this, &flag PARAMETERS]{func; flag.store(true);});   \
    while (!flag) QApplication::processEvents();\
    taskThread.join();                          \
    d.setValue(100)

#endif // WAITINGWINDOW_H
