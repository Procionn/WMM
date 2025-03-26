#ifndef COBJECT_H
#define COBJECT_H

#include "../../lang.h"
#include "../../methods.h"
#include "../../patterns/CSwitchButton.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QSplitter>
#include <QMouseEvent>
#include <string>
#include <vector>
#include <iostream>

class CSplitter;

class CObject : public QFrame
{
    Q_OBJECT
public:
    int index;
    std::string id;
    std::string name;
    std::string type;
    std::string version;
    bool toggl_condition = false;
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
private:
    void mousePressEvent(QMouseEvent* event) override;
signals:
    void ON(CObject* pointer);
    void OFF(CObject* pointer);
    void remove(CObject* pointer);
    void clicked(QMouseEvent* event, CObject* target);
    void rightClick(const QPoint& pos, CObject* target);
public slots:
    void ChangeOfState();
    void turnOff();
    void turnOn();
    void context(const QPoint& pos);
    void DELETE();
private slots:
    void INFO();
    void rightClickController(const QPoint& pos);
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
