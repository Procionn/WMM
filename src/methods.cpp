#include "methods.h"
#include "CONSTANTS.h"
#include "core.h"

#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#endif

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


void stc::fs::symlink (const std::filesystem::path& file, const std::filesystem::path& name) {
    std::filesystem::path newName = stc::string::replace(name, '\\', '/');
    std::filesystem::path newFile = stc::string::replace(file, '\\', '/');
    std::filesystem::create_directories(newName.parent_path());
    try {
#ifdef _WIN32
        if (!CreateSymbolicLinkA(newName.string().c_str(), newFile.string().c_str(), 0x2))
            std::cerr << "AHTUNG!!! I'm facking hate windows!!!"
                      << "\n" << newName << "\n" << newFile << std::endl;
#elif defined(__linux__)
        std::filesystem::create_symlink(newFile, newName);
#endif
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
