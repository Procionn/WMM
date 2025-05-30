#include "../hpp-archive.h"

#include "../methods.h"
#include <archive.h>
#include <archive_entry.h>

ArchiveReader::ArchiveReader(const std::filesystem::path& archivePath) {
    main = archive_read_new();
    archive_read_support_filter_all(main);
    archive_read_support_format_all(main);
    error = archive_read_open_filename(main, archivePath.string().c_str(), 10240);


}



ArchiveReader::~ArchiveReader () {
    error = archive_read_free(main);
    if (error != ARCHIVE_OK)
        stc::cerr("HPP_ARCHIVE ERROR: error in destructor");
}




std::vector<archive_entry> ArchiveReader::get_data () {
    return list;
}
