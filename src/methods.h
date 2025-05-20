#ifndef METHODS_H
#define METHODS_H

#include <string>
#include <filesystem>

namespace stc {
    namespace string {
        void replace (std::string& input, const char replaceable, const char target);
        std::filesystem::path replace (const std::filesystem::path& input, const char& replaceable, const char& target);
        std::string get_name(const std::string& path);
    }
    namespace cwmm { // Constants WMM
        std::string backup_path ();
        std::string ram_preset ();
        std::string ram_preset (const std::string& name);
        std::string ram_collection ();
        std::string ram_collection (const std::string& name);
        std::string modsURL(const std::string& id);
        std::string ram_mods();
        std::string ram_mods(const std::string& name);
        std::string ram_mods_info(const std::string& name);
    }
    namespace fs {   // file system
        void symlink(const std::filesystem::path& file, const std::filesystem::path& name);
        void remove_all(const std::filesystem::path& path);
    }
    namespace net {
        void openURL (const std::string& url);
    }
    namespace wmm {
        void new_object(const std::filesystem::path& path);
    }
}

#endif // METHODS_H
