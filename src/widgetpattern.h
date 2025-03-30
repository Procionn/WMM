#ifndef WIDGETPATTERN_H
#define WIDGETPATTERN_H

#include "dialog_window/CNewObjectDialog.h"
#include "patterns/ERRORdialog.h"
#include "patterns/CScrollWindow.h"
#include "patterns.h"
#include "dnd.h"
#include "window/modlist/cobject.h"
#include "window/modlist/cobjectscontainer.h"

#include <string>
#include <vector>
#include <QDialog>
#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QLabel>

class CObjectList : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout* objectList;
    std::string targetName;
    bool TypeTarget = false;
    
    CObjectList();
    void CreteObject(std::string name);
    void updateList(std::string toggledButton = "");
    void render();
signals:
    void objectChoosed(CObjectsButton* pointer, bool type);
    void remove();
public slots:
    void newObject(CNewObjectDialog* dialog);
private:
    std::vector<CObjectsButton*> list;
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

#endif // WIDGETPATTERN_H
