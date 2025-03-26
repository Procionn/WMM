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

class CScrollWindow : public QScrollArea
{
    Q_OBJECT
public:
    CScrollWindow(QWidget* parent,     QVBoxLayout* child);
    CScrollWindow(QWidget* parent,     QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QHBoxLayout* child);
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
