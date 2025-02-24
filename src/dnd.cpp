#include "dnd.h"
#include "CONSTANTS.h"
#include "core.h"

#include <QLabel>
#include <iostream>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <regex>

#include <archive.h>
#include <archive_entry.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdexcept>

CDND::CDND(QWidget* parent, std::string name) : QLabel(parent) {
    this->setText(QString::fromStdString(name));
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setMinimumHeight(50);
    setMaximumHeight(70);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
}

CDND::CDND(QVBoxLayout* parent, std::string name) {
    parent->addWidget(this);
    this->setText(QString::fromStdString(name));
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setMinimumHeight(50);
    setMaximumHeight(70);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
}


void CDND::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void CDND::dropEvent(QDropEvent* e) {
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        std::cout << "Dropped file:" << fileName.toStdString() << std::endl;
        try {
            ArchiveExtractor extractor(fileName.toStdString());
            std::string modsDir = MODS + CConfigs::CONFIG_GAME + "/" + extractor.regex(fileName.toStdString());
            std::filesystem::path fsPath = modsDir;
             if (std::filesystem::exists(modsDir)) {
                std::filesystem::remove_all(fsPath);
            }
            extractor.modInfoSave();
            extractor.extractAll(modsDir);
            
            std::string dad = ARCHIVE + CConfigs::CONFIG_GAME + "/" + extractor.id; // deep archive directory
            std::filesystem::create_directories(dad);
            std::ofstream daf(dad + "/" + extractor.version + EXPANSION2);          // deep archive file
            for (const auto& entry : std::filesystem::recursive_directory_iterator(modsDir)) {
                if (std::filesystem::is_regular_file(entry.path())) {
                    std::filesystem::path relative = std::filesystem::relative(entry.path(), fsPath);
                    daf << relative.generic_string() << "\n";
                }
            }
            emit launch();
            daf.close();
            
            std::cout << "The archive has been successfully extracted to the folder: " << modsDir << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "ERROR: " << ex.what() << std::endl;
        }
    }
}





ArchiveExtractor::ArchiveExtractor(const std::string& filename) {
    archive = archive_read_new();
    if (!archive) throw std::runtime_error("Failed to create archive object");
    
    archive_read_support_format_all(archive);
    archive_read_support_filter_all(archive);
    
    if (archive_read_open_filename(archive, filename.c_str(), 10240) != ARCHIVE_OK) {
        std::string error = archive_error_string(archive);
        archive_read_free(archive);
        throw std::runtime_error("Archive opening error: " + error);
    }
}

ArchiveExtractor::~ArchiveExtractor() {
    if (archive) {
        archive_read_free(archive);
    }
}

std::string ArchiveExtractor::regex (std::string ArchiveName) {
    size_t part = ArchiveName.find_last_of('/');
    std::string nameArchive = ArchiveName.substr(part + 1); 
    std::regex archiveRegex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?\.(\w+)$)");
    std::smatch matches;
    if (std::regex_match(nameArchive, matches, archiveRegex)) {
        name               = matches[1];
        id                 = matches[2];
        version            = matches[3];
        // uniqueNumber    = matches[4];
        // extension       = matches[5];
        return name;
        
    }
    else std::cout << "ERROR:  REGULAR NAME ERROR" << std::endl;
    std::abort();
    return NULL;
}

void ArchiveExtractor::modInfoSave() {
    std::string path = MODS + CConfigs::CONFIG_GAME + "/" + name + "/" + WML + name + EXPANSION2;
    std::filesystem::path dir = path;
    std::filesystem::create_directories(dir.parent_path());
    
    std::ofstream file(path.c_str());
    file << name << "\n";
    file << version << "\n";
    file << id << "\n";
    file.close();
}

void ArchiveExtractor::extractAll(const std::string& outputDir) {
    std::filesystem::create_directories(outputDir);
    struct archive_entry* entry;
    int r;
    while ((r = archive_read_next_header(archive, &entry)) == ARCHIVE_OK) {
        std::string entryPath = (std::filesystem::path(outputDir) / archive_entry_pathname(entry)).generic_string();
        
        if      (archive_entry_filetype(entry) == AE_IFDIR) extractDirectory(entryPath, entry);
        else if (archive_entry_filetype(entry) == AE_IFREG) extractFile(entryPath, entry);
        else    std::cerr << "Raw record type: " << entryPath << "\n";
        
        archive_read_data_skip(archive); // 
    }
    
    if (r != ARCHIVE_EOF) throw std::runtime_error("Archive reading error: " + std::string(archive_error_string(archive)));
}

void ArchiveExtractor::extractDirectory(const std::string& path, struct archive_entry* entry) {
    std::cout << "Создание директории: " << path << "\n";
    
    std::filesystem::create_directories(path);
}

void ArchiveExtractor::extractFile(const std::string& path, struct archive_entry* entry) {
    std::cout << "Extracting the file: " << path << "\n";
    std::filesystem::path fsPath = path;
    std::filesystem::create_directories(fsPath.parent_path());
    std::ofstream outFile(path, std::ios::binary | std::ios::trunc);
    if (!outFile) throw std::runtime_error("File creation error: " + path);
    
    const void* buffer;
    size_t size;
    la_int64_t offset;
    
    while (archive_read_data_block(archive, &buffer, &size, &offset) == ARCHIVE_OK) {
        outFile.write(static_cast<const char*>(buffer), size);
    }
    outFile.close();
}
