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
#ifndef UNIFICATOR_H
#define UNIFICATOR_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <hpp-archive.h>
#include "../patterns/CLinkTumbler.h"

class questioner final : public QWidget
{
    Q_OBJECT

public:
    questioner();
};


class choicer final : public QWidget
{
    Q_OBJECT

public:
    choicer(void*);
    std::string* string;

private slots:
    void buttonClick(CToggledButton*, const std::string&);
};


class unificator final : public QDialog
{
    Q_OBJECT

    QPushButton* back;
    QPushButton* next;
    questioner* firstPage;
    choicer* secondPage;
    bool status = false;
    std::string* string;

    unificator(void*);

public:
    static std::string start(void*);

private slots:
    void back_click();
    void next_click();
};


class CustomArchive final : public ArchiveWriter
{
protected:
    void open() override;

public:
    CustomArchive(const std::filesystem::path&, const std::filesystem::path&);
};

#endif // UNIFICATOR_H
