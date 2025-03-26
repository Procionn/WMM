#include "cobjectscontainer.h"

CObjectsContainer::CObjectsContainer() {
    list = new QVBoxLayout;
    QWidget* scrolledWidget = new QWidget;

    list->setAlignment(Qt::AlignTop);
    setWidgetResizable(true);
    setWidget(scrolledWidget);
    scrolledWidget->setLayout(list);

}

void CObjectsContainer::add (CObject* target) {
    list->addWidget(target);
    childList.emplace_back(target);
    connect(target, &CObject::clicked, this, &CObjectsContainer::controller);
    connect(target, &CObject::rightClick, this, &CObjectsContainer::rightClickController);
}

void CObjectsContainer::clear () {
    for (int i = 0; i < childList.size(); ++i)
        delete childList[i];
    childList.clear();
}


void CObjectsContainer::slot (CObject* target) {
    if (target == lastTarget) return;
    if (lastTarget != nullptr) {
        int thiscounter;
        int lastcounter;
        for (int i = 0; i < childList.size(); ++i) {
            if (childList[i] == target)
                thiscounter = i;
            if (childList[i] == lastTarget)
                lastcounter = i;
        }
        int max = std::max(thiscounter, lastcounter);
        int min = std::min(thiscounter, lastcounter);
        for (int i = min; i != max; ++i)
            childList[i]->turnOn();
        childList[max]->turnOn();
    }
    else {
        target->ChangeOfState();
        lastTarget = target;
    }
}

void CObjectsContainer::reset () {
    for (int i = 0; i < childList.size(); ++i) {
        childList[i]->turnOff();
        lastTarget = nullptr;
    }
}

void CObjectsContainer::controller (QMouseEvent* event, CObject* target) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ControlModifier) {
            target->ChangeOfState();
            lastTarget = target;
        }
        else if (event->modifiers() == Qt::ShiftModifier)
            slot(target);
        else reset();
    }
}

void CObjectsContainer::rightClickController (const QPoint& pos, CObject* target) {
    if (lastTarget == nullptr)
        target->context(pos);
    else {
        QMenu* contextMenu = new QMenu(this);
        QAction *action1 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_DELETE));
        connect(action1, &QAction::triggered, this, &CObjectsContainer::deletionSignals);
        contextMenu->exec(this->mapToGlobal(pos));
    }
}

void CObjectsContainer::deletionSignals () {
    std::vector<CObject*> newVector;
    for (CObject* target : childList) {
        if (target->toggl_condition)
            target->DELETE();
        else newVector.emplace_back(target);
    }
    childList.clear();
    childList = std::move(newVector);
}
