#ifndef COBJECTSCONTAINER_H
#define COBJECTSCONTAINER_H

#include "cobject.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <vector>
#include <algorithm>

class CObjectsContainer : public QScrollArea
{
    Q_OBJECT
public:
    std::vector<CObject*> childList;
    QVBoxLayout* list;
private:
    CObject* lastTarget = nullptr;
public:
    CObjectsContainer();
    void add(CObject* target);
    void clear();
private slots:
    void controller(QMouseEvent* event, CObject* target);
    void slot(CObject* target);
    void reset();
    void rightClickController(const QPoint& pos, CObject* target);
    void deletionSignals ();
};

#endif // COBJECTSCONTAINER_H
