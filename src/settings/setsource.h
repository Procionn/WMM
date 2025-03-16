#ifndef SETSOURCE_H
#define SETSOURCE_H

#include "../patterns.h"
#include "../dialogs.h"
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
};

#endif // SETSOURCE_H
