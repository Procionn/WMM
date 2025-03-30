#ifndef WINDOW_H
#define WINDOW_H

#include "dialog_window/cinpad/CInpad.h"
#include "widgetpattern.h"

#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class Window : public QMainWindow
{
Q_OBJECT
public:
    CNewObjectDialog* newObjectDialog;
    CObjectList* ObjectList;
    CContentList* ContentWidget;
    CObjectsButton* target = nullptr;
    CInpad* inpad;
    
    Window();
public slots:
    void NewObjectDialog();
    void settings();
    void updatePointer(CObjectsButton* pointer);
    void inpadShow();
};

#endif // WINDOW_H
