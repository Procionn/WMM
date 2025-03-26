#include "patterns.h"
#include "lang.h"
#include "methods.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QMenu>
#include <string>
#include <iostream>
#include <vector>


CScrollWindow::CScrollWindow (QWidget* parent, QVBoxLayout* child) : QScrollArea(parent) {
    QVBoxLayout* lay = new QVBoxLayout(parent);
    lay->addWidget(this);
    setWidgetResizable(true);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}
CScrollWindow::CScrollWindow (QWidget* parent, QHBoxLayout* child) : QScrollArea(parent) {
    QVBoxLayout* lay = new QVBoxLayout(parent);
    lay->addWidget(this);
    setWidgetResizable(true);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}

CScrollWindow::CScrollWindow (QVBoxLayout* parent, QVBoxLayout* child) {
    setWidgetResizable(true);
    parent->addWidget(this);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}
CScrollWindow::CScrollWindow (QHBoxLayout* parent, QVBoxLayout* child) {
    setWidgetResizable(true);
    parent->addWidget(this);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}
CScrollWindow::CScrollWindow (QHBoxLayout* parent, QHBoxLayout* child) {
    setWidgetResizable(true);
    parent->addWidget(this);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}
CScrollWindow::CScrollWindow (QVBoxLayout* parent, QHBoxLayout* child) {
    setWidgetResizable(true);
    parent->addWidget(this);
    QWidget* scrolledWidget = new QWidget;
    this->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}





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
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CObjectsButton::context);
}


void CObjectsButton::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_DELETE));
    connect(action1, &QAction::triggered, this, &CObjectsButton::DELETE);
    contextMenu->exec(this->mapToGlobal(pos));
}

void CObjectsButton::DELETE() {
    if (this->type) std::filesystem::remove(stc::cwmm::ram_preset(this->name));
    else            std::filesystem::remove(stc::cwmm::ram_collection(this->name));
    emit remove(this);
}







CInpadsButton::CInpadsButton(QVBoxLayout* parent, std::string setName, bool type) : CToggledButton(setName){
    this->type = type;
    parent->addWidget(this);
}
