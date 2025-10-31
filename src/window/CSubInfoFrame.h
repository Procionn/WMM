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
#ifndef CSUBINFOFRAME_H
#define CSUBINFOFRAME_H

#include <QHBoxLayout>
#include <QSplitter>
#include <vector>
class CSortedFrame;

class CSubInfoFrame : public QHBoxLayout
{
    Q_OBJECT
    std::vector<CSortedFrame*> list;

public:
    QSplitter* spl1;
    QSplitter* spl2;

    CSubInfoFrame(QVBoxLayout* parent = nullptr);

signals:
    void filter_changed(const int);

protected slots:
    void reset(CSortedFrame*);
};

#endif // CSUBINFOFRAME_H
