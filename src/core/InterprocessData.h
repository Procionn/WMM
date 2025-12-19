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
#ifndef INTERPROCESSDATA_H
#define INTERPROCESSDATA_H
#include <stdint.h>

enum class ComandList {
    kill = 0,
    dir_cmp,
    symlnk_del,
    symlnk_create
};


struct IpcHeader {
    uint32_t hash = 0xDEAFBEDE;
    uint32_t dataSize;
    ComandList comand;
    uint16_t version = 1;
};

#endif // INTERPROCESSDATA_H
