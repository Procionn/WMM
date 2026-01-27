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

#define t(code)                 \
try{                            \
    code;                       \
} catch(std::exception& e) {    \
    cerr(e.what());             \
}

namespace {
    static Core* ptr;

    template<typename T>
    void cerr (T t) {
        stc::cerr("Error in WMM-API:");
        stc::cerr(t);
    }
}

void WMM::APICore::start_api(Core* p) {
    ptr = p;
}

WMM::APICore::APICore() {
    ptr = &Core::get();
}

void WMM::APICore::update_lang() {
    t(ptr->update_lang())
}
QString WMM::APICore::tr (const QString& key) {
    t(return QString::fromStdString(ptr->lang.at(key.toStdString())))
    return"";
}
void WMM::APICore::load_new_lang_pack (const QString& path) {
    // t(ptr->load_lang_pack(path.toStdString()))
    stc::cerr(path.toStdString());
}
QString WMM::APICore::get_config (const QString& key) {
    t(return QString::fromStdString(ptr->configs.at(key.toStdString())))
    return"";
}
void WMM::APICore::config_reader() {
    t(ptr->config_reader())
}
void WMM::APICore::overwriting_config_data() {
    t(ptr->overwriting_config_data())
}
void WMM::APICore::set_default (const QString& key, const QString& value){
    t(ptr->set_default(key.toStdString(), value.toStdString()))
}
QString WMM::APICore::get_game_config(){
    t(return QString::fromStdString(ptr->configs.at("WMM_CONFIG_GAME")))
    return"";
}

void WMM::APICore::update_data_from_file(){
    t(ptr->update_data_from_file())
}
void WMM::APICore::save_game_path(const QString& path){
    t(ptr->save_game_path(path.toStdString()))
}
void WMM::APICore::game_dir_backup(){
    t(ptr->game_dir_backup())
}
void WMM::APICore::game_recovery(){
    t(ptr->game_recovery())
}

void WMM::APICore::restorer(){
    t(ptr->restorer())
}
void WMM::APICore::symlink_deliting(){
    t(ptr->symlink_deliting())
}
void WMM::APICore::symlink_creating (const QString& targetCollection){
    t(ptr->symlink_creating(targetCollection.toStdString()))
}

void WMM::APICore::exporter (const QString& name, const bool monolith){
    t(ptr->exporter(name.toStdString(), monolith))
}
void WMM::APICore::importer (const QString& path){
    t(ptr->importer(path.toStdString()))
}

void WMM::APICore::collector (const QString& name, bool type){
    t(ptr->collector(name.toStdString(), type))
}
