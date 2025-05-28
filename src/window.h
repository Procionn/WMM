#ifndef WINDOW_H
#define WINDOW_H

#include "dialog_window/cinpad/CInpad.h"
#include "window/mainWidgets.h"
#include "window/CMenuBar.h"
#include "settings/settings.h"

#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class Window : public QMainWindow
{
Q_OBJECT
    CSettings* settingsWindow = nullptr;

    void closeEvent(QCloseEvent *event) override;
public:
    CNewObjectDialog* newObjectDialog;
    CObjectList* ObjectList;
    CContentList* ContentWidget;
    CObjectsButton* target = nullptr;
    CInpad* inpad;
    
    Window();
    ~Window();
public slots:
    void NewObjectDialog();
    void settings();
    void updatePointer(CObjectsButton* pointer);
    void inpadShow();
private slots:
    void inpad_reset();
    void grounding(CObjectsButton* pointer);
};

#endif // WINDOW_H
