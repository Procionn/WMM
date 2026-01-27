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
#pragma once
#include <QString>
#include <QList>
#include <cstdint>
#include <QtCore/qglobal.h>

#if defined(WMMAPI)
    #define WMMAPI_EXPORT Q_DECL_EXPORT
#else
    #define WMMAPI_EXPORT Q_DECL_IMPORT
#endif

class ModManager;
class ModInfo;
class Mod;
namespace WMM {

    WMMAPI_EXPORT const uint64_t get_localId (const ModInfo*);
    WMMAPI_EXPORT QString get_mod_version (const ModInfo*);

    WMMAPI_EXPORT QString recommended_version (const Mod*);
    WMMAPI_EXPORT uint64_t get_modId (const Mod*);
    WMMAPI_EXPORT QList<ModInfo> get_versions (const Mod*);


    class WMMAPI_EXPORT APIModManager {
    public:
        static void start_api(ModManager* p);
        void add (const uint64_t& modId, const QString modVersion,
                  const QString modName);
        void add (const uint64_t& modId, QString& modVersion,
                  const QString& modName, const QString&);
        const QList<Mod*> all_mods_list();
        const QList<QString> all_versions_list (const uint64_t& modId);

        void update();
        void flush();
        bool get_copy();
        void set_copy (const bool&);

        void load (const QString& path);

        bool exists (const uint64_t id,  const QString& version);
        bool exists (const QString& name, const QString& version);

        void remove (const uint64_t id);
        void remove (const uint64_t id,  const QString& version);
        void remove (const QString& name);
        void remove (const QString& name, const QString& version);

        QString get_path (const uint64_t id);
        QString get_path (const uint64_t id,     const QString& version);
        QString get_log_path (const uint64_t id, const QString& version);
        QString get_path (const QString& name);
        QString get_path (const QString& name,     const QString& version);
        QString get_log_path (const QString& name, const QString& version);

        uint64_t mod_data_converter (const QString& modName);
        QString  mod_data_converter (const uint64_t modId);

        QString mod_recommended_version (const uint64_t modId);
        QString mod_recommended_version (const QString& modName);

    };

}
