#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>
#include <QMainWindow>

#include "../patterns/ERRORdialog.h"
#include "../core.h"
#include "../CONSTANTS.h"


class Window;

class CMenuBar : public QMenuBar
{
    Q_OBJECT
    Window* parent;
public:
    CMenuBar(Window* parent);
signals:
    void settings_click();
    void object_click();
    void content_click();
private slots:
    void assembling_clicked();
    void linking_clicked();
    void reassembly_clicked();
};

#endif // CMENUBAR_H
