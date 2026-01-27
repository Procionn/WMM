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

#define t(code)                 \
try{                            \
    code;                       \
} catch(std::exception& e) {    \
    cerr(e.what());             \
}

namespace {
    static ModManager* ptr;

    template<typename T>
    void cerr (T t) {
        stc::cerr("Error in WMM-API:");
        stc::cerr(t);
    }
}


void WMM::APIModManager::start_api(ModManager* p) {
    ptr = p;
}

const uint64_t get_localId (const ModInfo* ptr) {
    t(return ptr->localId)
}
QString get_mod_version (const ModInfo* ptr) {
    t(return QString::fromStdString(ptr->modVersion))
    return"";
}

QString recommended_version (const Mod* ptr) {
    t(return QString::fromStdString(ptr->recommended_version()))
    return"";
}
uint64_t get_modId (const Mod* ptr) {
    t(return ptr->modId)
}
QList<ModInfo> get_versions (const Mod* ptr) {
    if (ptr->versions) {
        t(
            auto data = *ptr->versions;
            QList<ModInfo> list;
            list.reserve(data.size());
            for (auto& entry : data)
                list.push_back(entry);
            return list;
        )
    }
    cerr("This object does not have \"versions\".");
    return{};
}





void WMM::APIModManager::add (const uint64_t& modId, const QString modVersion, const QString modName) {
    t(ptr->add(modId, modVersion.toStdString(), modName.toStdString()))
}
void WMM::APIModManager::add (const uint64_t& modId, QString& modVersion, const QString& modName, const QString& path) {
    t(
        std::string version = modVersion.toStdString();
        ptr->add(modId, version, modName.toStdString(), path.toStdString());
        modVersion = QString::fromStdString(version);
    )
}
const QList<Mod*> WMM::APIModManager::all_mods_list() {
    t(
        auto data = all_mods_list();
        QList<Mod*> list;
        list.reserve(data.size());
        for (auto& entry : data)
            list.push_back(entry);
        return list;
    )
    return {};
}
const QList<QString> WMM::APIModManager::all_versions_list (const uint64_t& modId){
    t(
        auto data = ptr->all_versions_list(modId);
        QList<QString> list;
        list.reserve(data.size());
        for (auto& entry : data)
            list.push_back(entry.data());
        return list
    )
    return {};
}

void WMM::APIModManager::update(){
    t(ptr->update())
}
void WMM::APIModManager::flush(){
    t(ptr->flush())
}
bool WMM::APIModManager::get_copy(){
    t(ptr->get_copy())
    return false;
}
void WMM::APIModManager::set_copy (const bool& type){
    t(ptr->set_copy(type);)
}

void WMM::APIModManager::load (const QString& path){
    t(ptr->load(path.toStdString()))
}

bool WMM::APIModManager::exists (const uint64_t id, const QString& version){
    t(return ptr->exists(id, version.toStdString()))
    return false;
}
bool WMM::APIModManager::exists (const QString& name, const QString& version){
    t(return ptr->exists(name.toStdString(), version.toStdString()))
    return false;
}

void WMM::APIModManager::remove (const uint64_t id){
    t(ptr->remove(id))
}
void WMM::APIModManager::remove (const uint64_t id,  const QString& version){
    t(ptr->remove(id, version.toStdString()))
}
void WMM::APIModManager::remove (const QString& name){
    t(ptr->remove(name.toStdString()))
}
void WMM::APIModManager::remove (const QString& name, const QString& version){
    t(ptr->remove(name.toStdString(), version.toStdString()))
}

QString WMM::APIModManager::get_path (const uint64_t id){
    t(return QString::fromStdString(ptr->get_path(id)))
    return"";
}
QString WMM::APIModManager::get_path (const uint64_t id, const QString& version){
    t(return QString::fromStdString(ptr->get_path(id, version.toStdString())))
    return"";
}
QString WMM::APIModManager::get_log_path (const uint64_t id, const QString& version){
    t(return QString::fromStdString(ptr->get_log_path(id, version.toStdString())))
    return"";
}
QString WMM::APIModManager::get_path (const QString& name){
    t(return QString::fromStdString(ptr->get_path(name.toStdString())))
    return"";
}
QString WMM::APIModManager::get_path (const QString& name, const QString& version){
    t(return QString::fromStdString(ptr->get_path(name.toStdString(), version.toStdString())))
    return"";
}
QString WMM::APIModManager::get_log_path (const QString& name, const QString& version){
    t(return QString::fromStdString(ptr->get_log_path(name.toStdString(), version.toStdString())))
    return"";
}

uint64_t WMM::APIModManager::mod_data_converter (const QString& modName){
    t(return ptr->mod_data_converter(modName.toStdString()))
    return 0;
}
QString WMM::APIModManager::mod_data_converter (const uint64_t modId){
    t(return QString::fromStdString(ptr->mod_data_converter(modId)))
    return"";
}

QString WMM::APIModManager::mod_recommended_version (const uint64_t modId){
    t(return QString::fromStdString(ptr->mod_recommended_version(modId)))
    return"";
}
QString WMM::APIModManager::mod_recommended_version (const QString& modName){
    t(return QString::fromStdString(ptr->mod_recommended_version(modName.toStdString())))
    return"";
}
