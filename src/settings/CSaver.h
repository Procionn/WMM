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
#ifndef CSAVER_H
#define CSAVER_H

#include <functional>
#include <vector>

class CSaver
{
public:
    void add_task(const char priority, std::function<void()> task);
    void run_all();
    void clear();

private:
    struct Task
    {
        char priority;
        std::function<void()> func;
    };

    std::vector<Task> tasks;
};

#endif // CSAVER_H
