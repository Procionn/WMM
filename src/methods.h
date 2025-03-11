#ifndef METHODS_H
#define METHODS_H

#include <string>
#include <filesystem>

namespace stc {
    namespace string {
        void replace (std::string& input, char replaceable, char target);
        std::filesystem::path replace (const std::filesystem::__cxx11::path input, char replaceable, char target);
    }
    namespace cwmm { // Constants WMM
    std::string backup_path ();
    }
    namespace fs {   // file system
        void symlink(const std::filesystem::path& file, const std::filesystem::path& name);
    }
}

#endif // METHODS_H
