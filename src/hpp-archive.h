#pragma once

#include <filesystem>
#include <vector>

class ArchiveReader
{
    struct archive* main;
    struct archive_entry* object;
    int error;

protected:
    std::vector<archive_entry> list;

public:
    ArchiveReader(const std::filesystem::path& archivePath);

    ~ArchiveReader();

    std::vector<archive_entry> get_data();
};


