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
#include <QLineEdit>
#include <QLabel>
#include <string>
#include <hpp-archive.h>
#include "../patterns/CLinkTumbler.h"

class questioner final : public QWidget
{
    Q_OBJECT

public:
    questioner(const std::string& name, QPushButton*);
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


class renamer final : public QWidget
{
    Q_OBJECT

public:
    QLineEdit* string;
    QLabel* errorLabel;

    renamer(const std::string& version);
};


class unificator final : public QDialog
{
    Q_OBJECT

    QPushButton* back;
    QPushButton* next;
    QPushButton* newVersion;
    questioner* firstPage;
    choicer* secondPage;
    renamer* thirdPage;
    unsigned char status = 0;
    std::string* string;
    uint64_t modId;

    unificator(void*, const std::string& name, const std::string& version, const uint64_t& id);
    void closeEvent(QCloseEvent* e) override;

protected slots:
    void reject() override;

public:
    static std::string start(void*, const std::string& name,
                             const std::string& version, const uint64_t& id);

private slots:
    void back_click();
    void next_click();
    void version_click();
};


class CustomArchive final : public ArchiveWriter
{
protected:
    void open() override;

public:
    CustomArchive(const std::filesystem::path&, const std::filesystem::path&);
};

#endif // UNIFICATOR_H
