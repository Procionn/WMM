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
#include "winrootbackend.h"


int main (int argc, char** argv) {
    QCoreApplication app(argc, argv);
    QString pipe, token;

    for (const QString& arg : app.arguments()) {
        if (arg.startsWith("--pipe="))
            pipe = arg.mid(7);
        else if (arg.startsWith("--token="))
            token = arg.mid(8);
    }

    if (pipe.isEmpty() || token.isEmpty())
        return 1;
    winrootbackend server(pipe, token);

    return app.exec();
}
