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
#include "Core.h"
#include "../core.h"
#include "../methods.h"


namespace {
    static Core* ptr;
}

void start_core_api(Core* p) {
    ptr = p;
}

void WMM::APICore::update_lang() {
    ptr->update_lang();
}
QString WMM::APICore::tr (const QString& key) {
    return QString::fromStdString(ptr->lang.at(key.toStdString()));
}
void WMM::APICore::load_new_lang_pack (const QString& path) {
    // ptr->load_lang_pack(path.toStdString());
}
QString WMM::APICore::get_config (const QString& key) {
    return QString::fromStdString(ptr->configs.at(key.toStdString()));
}
void WMM::APICore::config_reader() {
    ptr->config_reader();
}
void WMM::APICore::overwriting_config_data() {
    ptr->overwriting_config_data();
}
void WMM::APICore::set_default (const QString& key, const QString& value){
    ptr->set_default(key.toStdString(), value.toStdString());
}
QString WMM::APICore::get_game_config(){
    return QString::fromStdString(ptr->configs.at("WMM_CONFIG_GAME"));;
}

void WMM::APICore::update_data_from_file(){
    ptr->update_data_from_file();
}
void WMM::APICore::save_game_path(const QString& path){
    ptr->save_game_path(path.toStdString());
}
void WMM::APICore::game_dir_backup(){
    ptr->game_dir_backup();
}
void WMM::APICore::game_recovery(){
    ptr->game_recovery();
}

void WMM::APICore::restorer(){
    ptr->restorer();
}
void WMM::APICore::symlink_deliting(){
    ptr->symlink_deliting();
}
void WMM::APICore::symlink_creating (const QString& targetCollection){
    ptr->symlink_creating(targetCollection.toStdString());
}

void WMM::APICore::exporter (const QString& name, const bool monolith){
    ptr->exporter(name.toStdString(), monolith);
}
void WMM::APICore::importer (const QString& path){
    ptr->importer(path.toStdString());
}

void WMM::APICore::collector (const QString& name, bool type){
    ptr->collector(name.toStdString(), type);
}
