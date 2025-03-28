#include "templatelist.h"

TemplateList::TemplateList(QWidget* parent) : QScrollArea(parent){
    list = new QVBoxLayout;
    QWidget* scrolledWidget = new QWidget;

    list->setAlignment(Qt::AlignTop);
    setWidgetResizable(true);
    setWidget(scrolledWidget);
    scrolledWidget->setLayout(list);
}

void TemplateList::connecting (CSmartObject* linkable) {
    connect(linkable, &CSmartObject::clicked,    [=]
            (QMouseEvent* event, CSmartObject* target)
            {controller(event, target);});
    connect(linkable, &CSmartObject::rightClick, [=]
            (const QPoint& pos, CSmartObject* target)
            {rightClickController(pos, target);});
}

void TemplateList::controller (QMouseEvent* event, CSmartObject* target) {

}

void TemplateList::rightClickController (const QPoint& pos, CSmartObject* target) {

}
