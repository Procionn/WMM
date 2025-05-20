#ifndef COBJECTSBUTTON_H
#define COBJECTSBUTTON_H

#include "../patterns/CLinkTumbler.h"

#include <string>
#include <QWidget>
#include <QMenu>


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

#endif // COBJECTSBUTTON_H
