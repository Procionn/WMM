#ifndef TEMPLATELIST_H
#define TEMPLATELIST_H

#include <QVBoxLayout>
#include <QScrollArea>

#include "CSmartObject.h"

class TemplateList : public QScrollArea
{
    Q_OBJECT
protected:
    QVBoxLayout* list;
public:
    TemplateList(QWidget* parent = nullptr);
protected:
    void connecting(CSmartObject* linkable);

    virtual void controller(QMouseEvent* event, CSmartObject* target);
    virtual void rightClickController(const QPoint& pos, CSmartObject* target);
};

#endif // TEMPLATELIST_H
