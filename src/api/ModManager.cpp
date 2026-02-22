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
#include "ModManager.h"
#include "../ModManager.h"
#include "../methods.h"

namespace {
    static ModManager* ptr;
}


void WMM::APIModManager::start_api(ModManager* p) {
    ptr = p;
}

const uint64_t get_localId (const ModInfo* ptr) {
    return ptr->localId;
}
QString get_mod_version (const ModInfo* ptr) {
    return QString::fromStdString(ptr->modVersion);
}

QString recommended_version (const Mod* ptr) {
    return QString::fromStdString(ptr->recommended_version());
}
uint64_t get_modId (const Mod* ptr) {
    return ptr->modId;
}
QList<ModInfo*> get_versions (const Mod* ptr) {
    if (ptr->versions) {
        auto data = *ptr->versions;
        QList<ModInfo*> list;
        list.reserve(data.size());
        for (auto& entry : data)
            list.push_back(entry);
        return list;
    }
    throw std::runtime_error("Error in WMM-API: This object does not have \"versions\".");
}





void WMM::APIModManager::add (const uint64_t& modId, const QString modVersion, const QString modName) {
    ptr->add(modId, modVersion.toStdString(), modName.toStdString());
}
const QList<Mod*> WMM::APIModManager::all_mods_list() {
    auto data = all_mods_list();
    QList<Mod*> list;
    list.reserve(data.size());
    for (auto& entry : data)
        list.push_back(entry);
    return list;
}
const QList<QString> WMM::APIModManager::all_versions_list (const uint64_t& modId){
    auto data = ptr->all_versions_list(modId);
    QList<QString> list;
    list.reserve(data.size());
    for (auto& entry : data)
        list.push_back(entry.data());
    return list;
}

void WMM::APIModManager::update(){
    ptr->update();
}
void WMM::APIModManager::flush(){
    ptr->flush();
}
bool WMM::APIModManager::get_copy(){
    return ptr->get_copy();
}
void WMM::APIModManager::set_copy (const bool& type){
    ptr->set_copy(type);
}

void WMM::APIModManager::load (const QString& path){
    ptr->load(path.toStdString());
}

bool WMM::APIModManager::exists (const uint64_t id, const QString& version){
    return ptr->exists(id, version.toStdString());
}
bool WMM::APIModManager::exists (const QString& name, const QString& version){
    return ptr->exists(name.toStdString(), version.toStdString());
}

void WMM::APIModManager::remove (const uint64_t id){
    ptr->remove(id);
}
void WMM::APIModManager::remove (const uint64_t id,  const QString& version){
    ptr->remove(id, version.toStdString());
}
void WMM::APIModManager::remove (const QString& name){
    ptr->remove(name.toStdString());
}
void WMM::APIModManager::remove (const QString& name, const QString& version){
    ptr->remove(name.toStdString(), version.toStdString());
}

QString WMM::APIModManager::get_path (const uint64_t id){
    return QString::fromStdString(ptr->get_path(id));
}
QString WMM::APIModManager::get_path (const uint64_t id, const QString& version){
    return QString::fromStdString(ptr->get_path(id, version.toStdString()));
}
QString WMM::APIModManager::get_log_path (const uint64_t id, const QString& version){
    return QString::fromStdString(ptr->get_log_path(id, version.toStdString()));
}
QString WMM::APIModManager::get_path (const QString& name){
    return QString::fromStdString(ptr->get_path(name.toStdString()));
}
QString WMM::APIModManager::get_path (const QString& name, const QString& version){
    return QString::fromStdString(ptr->get_path(name.toStdString(), version.toStdString()));
}
QString WMM::APIModManager::get_log_path (const QString& name, const QString& version){
    return QString::fromStdString(ptr->get_log_path(name.toStdString(), version.toStdString()));
}

uint64_t WMM::APIModManager::mod_data_converter (const QString& modName){
    return ptr->mod_data_converter(modName.toStdString());
}
QString WMM::APIModManager::mod_data_converter (const uint64_t modId){
    return QString::fromStdString(ptr->mod_data_converter(modId));
}

QString WMM::APIModManager::mod_recommended_version (const uint64_t modId){
    return QString::fromStdString(ptr->mod_recommended_version(modId));
}
QString WMM::APIModManager::mod_recommended_version (const QString& modName){
    return QString::fromStdString(ptr->mod_recommended_version(modName.toStdString()));
}
