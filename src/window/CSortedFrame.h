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
#ifndef CSORTEDFRAME_H
#define CSORTEDFRAME_H

#include <QLabel>

class CSortedFrame : public QLabel
{
    Q_OBJECT
    char state = 0;
    static inline const std::string v = "v";
    static inline const std::string n = "^";
    static inline const std::string spacing = "  ";
    const std::string name;

public:
    CSortedFrame(const std::string&);

protected:
    void mousePressEvent(QMouseEvent*) override;
    void set_up();
    void set_down();

public slots:
    void reset();

signals:
    void up();
    void down();
};

#endif // CSORTEDFRAME_H
