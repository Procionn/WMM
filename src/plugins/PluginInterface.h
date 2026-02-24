/*
 * Copyright (C) 2025 Procion ByProcion@gmail.com
 *
 *  This file is part of WMM-toolkit.
 *  This work is licensed under the terms of the MIT license.
 *
 *  This files is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 *
 */
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#include <QtPlugin>

class PluginInterface
{
protected:
    const bool demon = false;
    // If the extensions is demon - switch to true
    // If the extension is service - switch to false
    PluginInterface (const bool isDemon = false) : demon(isDemon) {}

public:
    virtual ~PluginInterface() = default;
    virtual void main() = 0;
    virtual QString name() = 0;
    virtual QString get_icon_path () { return ""; }
    virtual bool is_demon () { return demon; }
    virtual QString description () { return ""; }
};


#define PluginInterface_iid "org.WMM.PluginInterface/1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
