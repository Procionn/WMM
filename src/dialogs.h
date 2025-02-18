#ifndef DIALOGS_H
#define DIALOGS_H

#include "patterns.h"

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>

class CFastDialog : public QDialog
{
    Q_OBJECT
public:
    QWidget* list;
    QPushButton* apply;
    
    CFastDialog();
signals:
    void applyClicked();
    void canselClicked();
};





class ERRORdialog : public CFastDialog
{
    Q_OBJECT
public:
    ERRORdialog(std::string label);
};





class CNewObjectDialog : public CFastDialog
{
    Q_OBJECT
public:
    QLineEdit* nameTab;
    CNewObjectDialog();
};





struct Cbox
{
    int index = 0;
    std::string path = "";
};




class CInpad : public CFastDialog
{
    Q_OBJECT
public:
    bool targetType = false;
    bool* callType;
    QVBoxLayout* newObjectList;
    CObjectsButton** target;
    
    CInpad(bool& type);
    void reset();
    void application(std::string targetName, bool targetType);
private:
    std::vector<CInpadsButton*> vlist;
    bool vector = false;
    QMenu* menu;
    QAction* presets;
    
    void render();
    void reader();
    void distributor();
    bool nameTest(Cbox existsElements[], int indicator, std::string str);
};


#endif // DIALOGS_H
