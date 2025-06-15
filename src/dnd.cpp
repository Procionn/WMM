#include "dnd.h"

#include "CONSTANTS.h"
#include "core.h"
#include "patterns/ERRORdialog.h"
#include "methods.h"

#include <iostream>
#include <hpp-archive.h>
#include <QDragEnterEvent>
#include <QMimeData>
#include <regex>



CDND::CDND(QVBoxLayout* parent, std::string name) {
    parent->addWidget(this);
    this->setText(QString::fromStdString(name));
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setMinimumHeight(70);
    setMaximumHeight(100);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setStyleSheet("background-color: #444b50; border-radius: 10px;");
}


void CDND::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void CDND::dropEvent(QDropEvent* e) {
    if (CConfigs::CONFIG_GAME != "None") {
        try {
            for (const QUrl &url : e->mimeData()->urls()) {
                std::string fileName = url.toLocalFile().toStdString();
                stc::cerr("Dropped file:" + fileName);

                regex(fileName);
                std::filesystem::path modsDir = stc::cwmm::ram_mods(name);
                if (std::filesystem::exists(modsDir))
                    stc::fs::remove_all(modsDir);
                modInfoSave(modsDir);

                ArchiveReader archive(fileName);
                archive.set_export_directory(modsDir);
                for (const auto* entry : archive)
                    archive.write_on_disk();


                std::filesystem::path path = ARCHIVE + Core::CONFIG_GAME + "/" + id;
                std::filesystem::create_directories(path);
                std::ofstream modStructFile(path / (version + EXPANSION2));
                for (const auto& entry : std::filesystem::recursive_directory_iterator(modsDir)) {
                    if (std::filesystem::is_regular_file(entry.path())) {
                        std::filesystem::path relative = std::filesystem::relative(entry.path(), modsDir);
                        modStructFile << relative.generic_string() << "\n";
                    }
                }
                modStructFile.close();

                stc::cerr("The archive has been successfully extracted to the folder: " + modsDir.string());
                emit launch();
            }
        }
        catch (const std::exception& e) {
            ERRORdialog* dialog = new ERRORdialog(std::string("DND ERROR: ") + e.what());
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R37"]);
}



void CDND::regex (const std::string& ArchiveName) {
    size_t part = ArchiveName.find_last_of('/');
    std::string nameArchive = ArchiveName.substr(part + 1);
    std::regex archiveRegex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?\.(\w+)$)");
    std::smatch matches;
    if (!std::regex_match(nameArchive, matches, archiveRegex))
        throw ("ERROR:  REGULAR NAME ERROR");
    name               = matches[1];
    id                 = matches[2];
    version            = matches[3];
    // uniqueNumber    = matches[4];
    // extension       = matches[5];
}


void CDND::modInfoSave(const std::filesystem::path& directory) {
    std::filesystem::path path = directory / (WML + name + EXPANSION2);
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path);
    file << name << "\n";
    file << version << "\n";
    file << id << "\n";
    file.close();
}
