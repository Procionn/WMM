#include "CSmartObject.h"

CSmartObject::CSmartObject(QWidget *parent) : QFrame(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            [=](const QPoint& pos){emit rightClick(pos, this);});
}

void CSmartObject::ChangeOfState () {
    if (toggl_condition)  {
        turnOff();
        toggl_condition = false;
    }
    else {
        turnOn();
        toggl_condition = true;
    }
}

void CSmartObject::mousePressEvent (QMouseEvent* event) {
    emit clicked(event, this);
}

void CSmartObject::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
    }
}

void CSmartObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
    }
}

void  CSmartObject::context (const QPoint& pos) {}
