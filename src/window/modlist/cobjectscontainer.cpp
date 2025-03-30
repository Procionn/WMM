#include "cobjectscontainer.h"

CObjectsContainer::CObjectsContainer() {
    list = new QVBoxLayout;
    QWidget* scrolledWidget = new QWidget;

    list->setAlignment(Qt::AlignTop);
    setWidgetResizable(true);
    setWidget(scrolledWidget);
    scrolledWidget->setLayout(list);

}

void CObjectsContainer::RMB (const QPoint& pos, CObject* target) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_DELETE));
    connect(action1, &QAction::triggered, this, &CObjectsContainer::deletionSignals);
    contextMenu->exec(this->mapToGlobal(pos));
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
