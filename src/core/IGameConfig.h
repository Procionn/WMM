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
#ifndef IGAMECONFIG_H
#define IGAMECONFIG_H

#include <string>
#include <filesystem>

class IGameConfig {
protected:
    IGameConfig() = default;

public:
    virtual ~IGameConfig() {}
    virtual void dir_comparison (const std::filesystem::path& file) = 0;
    virtual void symlink_deliting() = 0;
    virtual void symlink_creating(const std::string& targetCollection) = 0;
};


class WinGameConfig : public IGameConfig
{
public:
    virtual void dir_comparison (const std::filesystem::path& file) override;
    virtual void symlink_deliting() override;
    virtual void symlink_creating(const std::string& targetCollection) override;
};



class NixGameConfig : public IGameConfig
{
public:
    virtual void dir_comparison (const std::filesystem::path& file) override;
    virtual void symlink_deliting() override;
    virtual void symlink_creating(const std::string& targetCollection) override;
};

#endif // IGAMECONFIG_H
