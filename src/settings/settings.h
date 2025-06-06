#ifndef SETTINGS_H
#define SETTINGS_H

#include "../patterns/CLinkTumbler.h"
#include "setlang.h"
#include "setsource.h"
#include "setsupport.h"
#include "setextensions.h"
#include "collections.h"

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
    CLinkTumbler* collections;
    
    SObjects();
};





class SList : public QWidget
{
    Q_OBJECT
public:  
    SList();

    setsource* settings_source;
    setlang* settings_lang;
    setsupport* settings_support;
    setextensions* settings_extension;
    collections* settings_collections;

public slots:
    void sorce();
    void lang();
    void support();
    void extensions();
    void collection();
};





class CSettings : public QDialog
{
    Q_OBJECT
public:
    SObjects* sobjects;
    SList* settings_modules_list;
    
    CSettings();
private slots:
    void save();
};

#endif // SETTINGS_H
