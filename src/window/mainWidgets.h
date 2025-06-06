#ifndef MAINWIDGETS_H
#define MAINWIDGETS_H

#include "../dialog_window/CNewObjectDialog.h"
#include "../patterns/CScrollWindow.h"
#include "../window/modlist/cobjectscontainer.h"
#include "CObjectsButton.h"
#include "../dnd.h"

#include <string>
#include <filesystem>
#include <vector>
#include <QVBoxLayout>
#include <QSplitter>



class CObjectList : public QWidget
{
    Q_OBJECT
    std::vector<CObjectsButton*> list;
public:
    QVBoxLayout* objectList;
    std::string targetName;
    bool TypeTarget = false;

public:
    CObjectList();
    void CreteObject(std::string name);
    void updateList();
    void render();

private:
    void scan_directory (const std::filesystem::path& directory, const bool type, CObjectsButton*& lastTumbler);
signals:
    void objectChoosed(CObjectsButton* pointer, bool type);
    void remove(CObjectsButton* pointer);

public slots:
    void newObject(CNewObjectDialog* dialog);
};





class CContentList : public QWidget
{
    Q_OBJECT
public:
    QSplitter* spl1;
    QSplitter* spl2;
    CObjectsContainer* contentList;
    QWidget* contentWidget;
    CDND* dnd;

    std::string sPath;
    std::string targetName;
    bool targetType;

    CContentList();
public slots:
    void updateList(CObjectsButton* pointer, bool type);
    void clear();
private slots:
    void changeStatusOn(CObject* toggledElements);
    void changeStatusOff(CObject* toggledElements);
    void deleting(CObject* pointer);
};


#endif // MAINWIDGETS_H
