#ifndef SETTINGS_H
#define SETTINGS_H

#include "patterns.h"

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
    
    SObjects();
};





class SList : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout* firstlist;
    QVBoxLayout* lastlist;
    QString buffer;
    CLinkTumbler* target = nullptr;
    
    SList();
private:
    void chooseGame(QPushButton* parent);
    CLinkTumbler* tmptarget = nullptr;
public slots:
    void sorce();
    void lang();
    void support();
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
