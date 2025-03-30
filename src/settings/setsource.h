#ifndef SETSOURCE_H
#define SETSOURCE_H

#include "../patterns/CScrollWindow.h"
#include "../patterns/ERRORdialog.h"
#include "../patterns.h"
#include "../CONSTANTS.h"
#include "../lang.h"

#include <QWidget>

class setsource : public QWidget
{
    Q_OBJECT
private:
    CLinkTumbler* tmptarget = nullptr;
public:
    QVBoxLayout* firstlist;
    QVBoxLayout* lastlist;
    QString buffer;
    CLinkTumbler* target = nullptr;
public:
    setsource();
private slots:
    void chooseGame(QPushButton* parent);
    void chooseExe(QPushButton* dirBTN);
    void createBackup();
};

#endif // SETSOURCE_H
