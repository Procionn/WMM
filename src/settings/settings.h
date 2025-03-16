#ifndef SETTINGS_H
#define SETTINGS_H

#include "../patterns.h"
#include "setlang.h"
#include "setsource.h"
#include "setsupport.h"
#include "setextensions.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>

class SObjects : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout* list;
    CLinkTumbler* sorce;
    CLinkTumbler* lang;
    CLinkTumbler* support;
    CLinkTumbler* extensions;
    
    SObjects();
};





class SList : public QWidget
{
    Q_OBJECT
public:  
    SList();

    setsource* ss;
    setlang* sl;
    setsupport* sup;
    setextensions* se;

public slots:
    void sorce();
    void lang();
    void support();
    void extensions();
};





class CSettings : public QDialog
{
    Q_OBJECT
public:
    SObjects* sobjects;
    SList* slist;
    
    CSettings();
private slots:
    void save();
};

#endif // SETTINGS_H
