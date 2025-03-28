#ifndef COBJECT_H
#define COBJECT_H

#include "../../lang.h"
#include "../../methods.h"
#include "../../patterns/CSwitchButton.h"
#include "../../patterns/CSmartObject.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QSplitter>
#include <QMouseEvent>
#include <string>
#include <vector>
#include <iostream>

class CSplitter;

class CObject : public CSmartObject
{
    Q_OBJECT
public:
    int index;
    std::string id;
    std::string name;
    std::string type;
    std::string version;
protected:
    std::string untoggledColor = " background-color: #1e1e1e;";
    std::string toggledColor = " background-color: #444b52;";
public:
    QHBoxLayout* Box;
    QLabel* Lname;
    QLabel* Lversion;
    QLabel* Ltype;
    CSplitter* spl1;
    CSplitter* spl2;
    CSwitchButton* switcher;
public:
    CObject(const std::vector<std::string>& v);
signals:
    void ON(CObject* pointer);
    void OFF(CObject* pointer);
    void remove(CObject* pointer);
public slots:
    void turnOff() override;
    void turnOn() override;
    void context(const QPoint& pos) override;
    void DELETE();
private slots:
    void INFO();
};





class CSplitter : public QSplitter
{
    Q_OBJECT
public:
    CSplitter(QHBoxLayout* parent);
public slots:
    void moveSplitter(int pos, int index);
};

#endif // COBJECT_H
