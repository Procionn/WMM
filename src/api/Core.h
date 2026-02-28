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
#include <QtCore/qglobal.h>

#if defined(WMMAPI)
    #define WMMAPI_EXPORT Q_DECL_EXPORT
#else
    #define WMMAPI_EXPORT Q_DECL_IMPORT
#endif

class Core;
extern "C" WMMAPI_EXPORT void start_core_api(Core*);
namespace WMM {

    class WMMAPI_EXPORT APICore {
    public:
        static void update_lang();
        static QString tr (const QString& key);
        static void load_new_lang_pack (const QString& path);

        static QString get_config (const QString& key);
        static void config_reader();
        static void overwriting_config_data();
        static void set_default (const QString& key, const QString& value);
        static QString get_game_config();

        static void update_data_from_file();
        static void save_game_path(const QString& path);
        static void game_dir_backup();
        static void game_recovery();

        static void restorer();
        static void symlink_deliting();
        static void symlink_creating (const QString& targetCollection);

        static void exporter (const QString& name, const bool monolith);
        static void importer (const QString& path);

        static void collector (const QString& name, bool type);
    };

}
