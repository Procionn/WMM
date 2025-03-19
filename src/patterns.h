#ifndef PATTERNS_H
#define PATTERNS_H

#include "patterns/CToggledButton.h"
#include "patterns/CSwitchButton.h"
#include "patterns/CLinkTumbler.h"

#include <string>
#include <QFrame>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>

class CSplitter;

class CContentBox : public QFrame
{
    Q_OBJECT
public:
    int index;
    std::string aid;
    std::string aname;
    std::string atype;
    QHBoxLayout* Box;
    QLabel* name;
    QLabel* version;
    QLabel* type;
    CSwitchButton* switcher;
    CSplitter* spl1;
    CSplitter* spl2;
    CSplitter* spl3;
    
    CContentBox(QVBoxLayout* parent, std::vector<std::string>& v);
signals:
    void ON(CContentBox* pointer);
    void OFF(CContentBox* pointer);
    void remove(CContentBox* pointer);
private slots:
    void context(const QPoint &pos);
    void DELETE();
    void INFO();
};





class CScrollWindow : public QScrollArea
{
    Q_OBJECT
public:
    CScrollWindow(QWidget* parent, QVBoxLayout* child);
    CScrollWindow(QWidget* parent, QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QHBoxLayout* child);
};





class CSplitter : public QSplitter
{
    Q_OBJECT
public:
    CSplitter(QHBoxLayout* parent);
public slots:
    void moveSplitter(int pos, int index);
};





class CObjectsButton : public CLinkTumbler
{
    Q_OBJECT
public:
    bool type;
    CObjectsButton* link = nullptr;
    CObjectsButton(std::string name = "", CObjectsButton* linked = nullptr, QWidget* parent = nullptr);
private slots:
    void context(const QPoint &pos);
    void DELETE();
signals:
    void remove(CObjectsButton* button);
};





class CInpadsButton : public CToggledButton
{
    Q_OBJECT
public:
    bool type = false;
    CInpadsButton(QVBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
};

#endif // PATTERNS_H
