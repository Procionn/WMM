#include "CObjectsButton.h"

#include "../core.h"
#include "../methods.h"

CObjectsButton::CObjectsButton(std::string name, CObjectsButton* linked, QWidget* parent) : CLinkTumbler(name){
    this->name = name;
    if (linked != nullptr) {
        link = linked;
        connect(this, &CObjectsButton::clicked, linked, &CObjectsButton::reset);
        connect(linked, &CObjectsButton::clicked, this, &CObjectsButton::reset);
        while (linked->link != nullptr) {
            linked = linked->link;
            connect(this, &CObjectsButton::clicked, linked, &CObjectsButton::reset);
            connect(linked, &CObjectsButton::clicked, this, &CObjectsButton::reset);
        }
    }
    setMinimumHeight(35);
    SetLeftAlignment(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CObjectsButton::context);
}


void CObjectsButton::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Core::lang["LANG_BUTTON_DELETE"]));
    connect(action1, &QAction::triggered, this, &CObjectsButton::DELETE);
    contextMenu->exec(this->mapToGlobal(pos));
}

void CObjectsButton::DELETE() {
    if (type) std::filesystem::remove(stc::cwmm::ram_preset(name));
    else      std::filesystem::remove(stc::cwmm::ram_collection(name));
    emit remove(this);
}
