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
public:
    virtual ~PluginInterface() = default;
    virtual void plugin_main() = 0;
    virtual QString name() = 0;
};


#define PluginInterface_iid "org.WMM.PluginInterface/1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
