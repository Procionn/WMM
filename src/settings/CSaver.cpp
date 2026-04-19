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
#include "CSaver.h"

#include <algorithm>

void CSaver::add_task (const char priority, std::function<void()> task) {
    tasks.push_back({priority, std::move(task)});
}

void CSaver::run_all () {
    if (tasks.empty())
        return;
    std::sort(tasks.begin(), tasks.end(),
              [] (const Task& a, const Task& b) { return a.priority < b.priority; });

    for (const auto& t : tasks)
        if (t.func)
            t.func();

    tasks.clear();
}

void CSaver::clear () {
    tasks.clear();
}
