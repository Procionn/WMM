#include "lang.h"
#include "core.h"

#include <fstream>
#include <string>
#include <iostream>

std::string Lang::LANG_BUTTON_PRESETS;
std::string Lang::LANG_BUTTON_COLLECTIONS;
std::string Lang::LANG_BUTTON_MODS;
std::string Lang::LANG_BUTTON_SETTINGS;
std::string Lang::LANG_BUTTON_ADD_COMPONENTS;
std::string Lang::LANG_BUTTON_NEW_OBJECT;
std::string Lang::LANG_BUTTON_APPLY;
std::string Lang::LANG_BUTTON_CANSEL;
std::string Lang::LANG_BUTTON_ASSEMBLING;
std::string Lang::LANG_BUTTON_REASSEMBLY;
std::string Lang::LANG_BUTTON_SORCE;
std::string Lang::LANG_BUTTON_LANG;
std::string Lang::LANG_BUTTON_SUPPORT;
std::string Lang::LANG_BUTTON_LINKING;
std::string Lang::LANG_BUTTON_GAME_BACKUP;
std::string Lang::LANG_BUTTON_GAME_CLEAR;
std::string Lang::LANG_BUTTON_GAME_RECOVERY;
std::string Lang::LANG_LABEL_NAME;
std::string Lang::LANG_LABEL_R36;
std::string Lang::LANG_LABEL_R35;
std::string Lang::LANG_LABEL_R34;
std::string Lang::LANG_LABEL_R33;
std::string Lang::LANG_LABEL_R32;
std::string Lang::LANG_LABEL_R31;
std::string Lang::LANG_LABEL_R30;
std::string Lang::LANG_LABEL_DND;
std::string Lang::LANG_LABEL_ADD;
std::string Lang::LANG_LABEL_VERSION;
std::string Lang::LANG_LABEL_TYPE;
std::string Lang::LANG_LABEL_SWITCHER;
std::string Lang::LANG_BUTTON_ACCEPT;
std::string Lang::LANG_LABEL_DIRECTORY_CHOOSE;
std::string Lang::LANG_LABEL_GAME_CHOOSE;
std::string Lang::LANG_LABEL_ALL_FILE;
std::string Lang::LANG_LABEL_CHOOSE_GAME_FILE;
std::string Lang::LANG_LABEL_GAME_BACKUP;
std::string Lang::LANG_LABEL_GAME_CLEAR;
std::string Lang::LANG_LABEL_GAME_RECOVERY;

void Lang::set_lang() {
    std::ifstream readedFile (CConfigs::CONFIG_LANGUAGES.c_str());
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator)) {
             if (parameter == "LANG_BUTTON_COLLECTIONS")     LANG_BUTTON_COLLECTIONS     = indicator;
        else if (parameter == "LANG_BUTTON_PRESETS")         LANG_BUTTON_PRESETS         = indicator;
        else if (parameter == "LANG_BUTTON_MODS")            LANG_BUTTON_MODS            = indicator;
        else if (parameter == "LANG_BUTTON_SETTINGS")        LANG_BUTTON_SETTINGS        = indicator;
        else if (parameter == "LANG_BUTTON_ADD_COMPONENTS")  LANG_BUTTON_ADD_COMPONENTS  = indicator;
        else if (parameter == "LANG_BUTTON_NEW_OBJECT")      LANG_BUTTON_NEW_OBJECT      = indicator;
        else if (parameter == "LANG_BUTTON_APPLY")           LANG_BUTTON_APPLY           = indicator;
        else if (parameter == "LANG_BUTTON_CANSEL")          LANG_BUTTON_CANSEL          = indicator;
        else if (parameter == "LANG_BUTTON_ASSEMBLING")      LANG_BUTTON_ASSEMBLING      = indicator;
        else if (parameter == "LANG_BUTTON_REASSEMBLY")      LANG_BUTTON_REASSEMBLY      = indicator;
        else if (parameter == "LANG_BUTTON_SORCE")           LANG_BUTTON_SORCE           = indicator;
        else if (parameter == "LANG_BUTTON_LANG")            LANG_BUTTON_LANG            = indicator;
        else if (parameter == "LANG_BUTTON_SUPPORT")         LANG_BUTTON_SUPPORT         = indicator;
        else if (parameter == "LANG_BUTTON_ACCEPT")          LANG_BUTTON_ACCEPT          = indicator;
        else if (parameter == "LANG_BUTTON_LINKING")         LANG_BUTTON_LINKING         = indicator;
        else if (parameter == "LANG_BUTTON_GAME_BACKUP")     LANG_BUTTON_GAME_BACKUP     = indicator;
        else if (parameter == "LANG_BUTTON_GAME_CLEAR")      LANG_BUTTON_GAME_CLEAR      = indicator;
        else if (parameter == "LANG_BUTTON_GAME_RECOVERY")   LANG_BUTTON_GAME_RECOVERY   = indicator;
        else if (parameter == "LANG_LABEL_NAME")             LANG_LABEL_NAME             = indicator;
        else if (parameter == "LANG_LABEL_R36")              LANG_LABEL_R36              = indicator;
        else if (parameter == "LANG_LABEL_R35")              LANG_LABEL_R35              = indicator;
        else if (parameter == "LANG_LABEL_R34")              LANG_LABEL_R34              = indicator;
        else if (parameter == "LANG_LABEL_R33")              LANG_LABEL_R33              = indicator;
        else if (parameter == "LANG_LABEL_R32")              LANG_LABEL_R32              = indicator;
        else if (parameter == "LANG_LABEL_R31")              LANG_LABEL_R31              = indicator;
        else if (parameter == "LANG_LABEL_R30")              LANG_LABEL_R30              = indicator;
        else if (parameter == "LANG_LABEL_DND")              LANG_LABEL_DND              = indicator;
        else if (parameter == "LANG_LABEL_ADD")              LANG_LABEL_ADD              = indicator;
        else if (parameter == "LANG_LABEL_VERSION")          LANG_LABEL_VERSION          = indicator;
        else if (parameter == "LANG_LABEL_TYPE")             LANG_LABEL_TYPE             = indicator;
        else if (parameter == "LANG_LABEL_SWITCHER")         LANG_LABEL_SWITCHER         = indicator;
        else if (parameter == "LANG_LABEL_DIRECTORY_CHOOSE") LANG_LABEL_DIRECTORY_CHOOSE = indicator;
        else if (parameter == "LANG_LABEL_GAME_CHOOSE")      LANG_LABEL_GAME_CHOOSE      = indicator;
        else if (parameter == "LANG_LABEL_ALL_FILE")         LANG_LABEL_ALL_FILE         = indicator;
        else if (parameter == "LANG_LABEL_CHOOSE_GAME_FILE") LANG_LABEL_CHOOSE_GAME_FILE = indicator;
        else if (parameter == "LANG_LABEL_GAME_BACKUP")      LANG_LABEL_GAME_BACKUP      = indicator;
        else if (parameter == "LANG_LABEL_GAME_CLEAR")       LANG_LABEL_GAME_CLEAR       = indicator;
        else if (parameter == "LANG_LABEL_GAME_RECOVERY")    LANG_LABEL_GAME_RECOVERY    = indicator;
        // else if (parameter == "")  = indicator;
    }
}
