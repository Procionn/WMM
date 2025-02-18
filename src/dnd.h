#ifndef DND_H
#define DND_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <string>

class CDND : public QLabel
{
    Q_OBJECT
public:
    CDND(QWidget* parent = nullptr, std::string name = "");
    CDND(QVBoxLayout* parent = nullptr, std::string name = "");
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
signals:
    void launch();
};





class ArchiveExtractor {
public:
    ArchiveExtractor(const std::string& filename);
    ~ArchiveExtractor();
    
    void extractAll(const std::string& outputDir);
    std::string regex (std::string ArchiveName);
    void modInfoSave();
    std::string name;
    std::string id;
    std::string version;
private:
    struct archive* archive;
    
    void extractDirectory(const std::string& path, struct archive_entry* entry);
    void extractFile(const std::string& path, struct archive_entry* entry);
};

#endif // DND_H
