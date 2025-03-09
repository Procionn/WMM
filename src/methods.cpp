#include "methods.h"
#include "CONSTANTS.h"
#include "core.h"

#include <string>
#include <filesystem>
#include <algorithm>

void stc::string::replace (std::string& input, char replaceable, char target) {
    for (int i = 0; input[i]; ++i)
        if (input[i] == replaceable)
            input[i] = target;
}
std::filesystem::path stc::string::replace (const std::filesystem::path input, char replaceable, char target) {
    std::string sorce = input.string();
    std::replace(sorce.begin(), sorce.end(), '\\', '/');
    std::filesystem::path out = sorce;
    return out;
}


std::string stc::cwmm::backup_path () {
    return (GAME + "/" + CConfigs::CONFIG_GAME);
}
