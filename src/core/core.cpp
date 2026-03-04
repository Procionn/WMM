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
#include "../core.h"
#include <QLibrary>
#include <QDir>
#include "../methods.h"

typedef void (*func)(Core*);
Core::Core ()
    : CConfigs(),
      Lang(CConfigs::get_config("WMM_CONFIG_LANGUAGES")),
      CGameConfig(CConfigs::get_config("WMM_CONFIG_USE_EXTERNAL_MODULE"),
                  CConfigs::get_config("WMM_CONFIG_GAME")) {

    func f = (func)(QLibrary::resolve("./" LIB, "start_core_api"));
    if (f)
        f(this);
    else {
        stc::cerr("api not loaded");
        std::exit(12);
    }
}


Core& Core::get () {
    static Core singleton;
    return singleton;
}
