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

        void update_lang();
        QString tr (const QString& key);
        void load_new_lang_pack (const QString& path);

        QString get_config (const QString& key);
        void config_reader();
        void overwriting_config_data();
        void set_default (const QString& key, const QString& value);
        QString get_game_config();

        void update_data_from_file();
        void save_game_path(const QString& path);
        void game_dir_backup();
        void game_recovery();

        void restorer();
        void symlink_deliting();
        void symlink_creating (const QString& targetCollection);

        void exporter (const QString& name, const bool monolith);
        void importer (const QString& path);

        void collector (const QString& name, bool type);
    };

}
