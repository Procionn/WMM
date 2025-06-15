#ifndef DND_H
#define DND_H

#include <QVBoxLayout>
#include <QLabel>
#include <filesystem>

class CDND : public QLabel
{
    Q_OBJECT

    std::string name;
    std::string id;
    std::string version;

public:
    CDND(QVBoxLayout* parent = nullptr, std::string name = "");
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);

signals:
    void launch();

protected:
    void regex(const std::string& ArchiveName);
    void modInfoSave(const std::filesystem::path& directory);

};
#endif // DND_H
