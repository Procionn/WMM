#ifndef SETLANG_H
#define SETLANG_H

#include "../patterns.h"

#include <QWidget>
#include <QPushButton>

class setlang : public QWidget
{
    Q_OBJECT
public:
    CLinkTumbler* target;
public:
    setlang();
private slots:
    void chooseLang(QPushButton* parent);
};

#endif // SETLANG_H
