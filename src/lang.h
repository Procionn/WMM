#ifndef LANG_H
#define LANG_H
#include "core.h"

class Lang : public CBaseConfig
{
public:
    static std::string LANG_BUTTON_COLLECTIONS;
    static std::string LANG_BUTTON_PRESETS;
    static std::string LANG_BUTTON_MODS;
    static std::string LANG_BUTTON_SETTINGS;
    static std::string LANG_BUTTON_ADD_COMPONENTS;
    static std::string LANG_BUTTON_NEW_OBJECT;
    static std::string LANG_BUTTON_APPLY;
    static std::string LANG_BUTTON_ACCEPT;
    static std::string LANG_BUTTON_CANSEL;
    static std::string LANG_BUTTON_ASSEMBLING;
    static std::string LANG_BUTTON_REASSEMBLY;
    static std::string LANG_BUTTON_SORCE;
    static std::string LANG_BUTTON_LANG;
    static std::string LANG_BUTTON_SUPPORT;
    static std::string LANG_BUTTON_LINKING;
    static std::string LANG_BUTTON_GAME_BACKUP;
    static std::string LANG_BUTTON_GAME_CLEAR;
    static std::string LANG_BUTTON_GAME_RECOVERY;
    static std::string LANG_LABEL_NAME;
    static std::string LANG_LABEL_R36;
    static std::string LANG_LABEL_R35;
    static std::string LANG_LABEL_R34;
    static std::string LANG_LABEL_R33;
    static std::string LANG_LABEL_R32;
    static std::string LANG_LABEL_R31;
    static std::string LANG_LABEL_R30;
    static std::string LANG_LABEL_DND;
    static std::string LANG_LABEL_ADD;
    static std::string LANG_LABEL_VERSION;
    static std::string LANG_LABEL_TYPE;
    static std::string LANG_LABEL_SWITCHER;
    static std::string LANG_LABEL_DIRECTORY_CHOOSE;
    static std::string LANG_LABEL_GAME_CHOOSE;
    static std::string LANG_LABEL_ALL_FILE;
    static std::string LANG_LABEL_CHOOSE_GAME_FILE;
    static std::string LANG_LABEL_GAME_BACKUP;
    static std::string LANG_LABEL_GAME_CLEAR;
    static std::string LANG_LABEL_GAME_RECOVERY;
    void set_lang();
};

#endif // LANG_H
