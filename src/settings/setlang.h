#ifndef SETLANG_H
#define SETLANG_H

#include "../patterns/CScrollWindow.h"
#include "../patterns/CFastDialog.h"
#include "../patterns/CLinkTumbler.h"

#include <QWidget>
#include <QPushButton>

class setlang : public QWidget
{
    Q_OBJECT
public:
    CLinkTumbler* target = nullptr;
public:
    setlang();
private slots:
    void chooseLang(QPushButton* parent);
};

#endif // SETLANG_H
