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

CContentBox::CContentBox (QVBoxLayout* parent, std::vector<std::string>& v) {
    parent->addWidget(this);
    Box = new QHBoxLayout(this);
    index = std::stoi(v[0]);
    name = new QLabel(QString::fromStdString(v[1]));
    version = new QLabel(QString::fromStdString(v[2]));
    type = new QLabel(QString::fromStdString(v[3]));
    switcher = new CSwitchButton;
    switcher->setTheme("orange");
    switcher->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    if (v[5] == "1") switcher->isTarget(true);
    else             switcher->isTarget(false);
    aid = v[4];
    aname = v[1];
    atype = v[3];
    name->resize(400, 0);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);
    // or use it -> setFrameShadow(QFrame::Raised);
    spl1 = new CSplitter(Box);
    spl2 = new CSplitter(Box);
    spl1->addWidget(name);
    spl1->addWidget(spl2);
    spl2->addWidget(version);
    spl2->addWidget(type);
    Box->addWidget(switcher);
    connect(switcher, &CSwitchButton::toggled,   this, [=]{emit ON(this);});
    connect(switcher, &CSwitchButton::untoggled, this, [=]{emit OFF(this);});

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CContentBox::context);
}

void CContentBox::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_DELETE));
    connect(action1, &QAction::triggered, this, &CContentBox::DELETE);
    if (atype == "mod") {
        QAction *action2 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_INFO));
        connect(action2, &QAction::triggered, this, &CContentBox::INFO);
    }
    contextMenu->exec(this->mapToGlobal(pos));
}

void CContentBox::DELETE() {
    emit remove(this);
}

void CContentBox::INFO() {
    std::ifstream file(stc::cwmm::ram_mods_info(aname));
    std::string id;
    for (int i = 0; i != 3; ++i)
        std::getline(file, id);
    stc::net::openURL(stc::cwmm::modsURL(id));
}





CSplitter::CSplitter(QHBoxLayout* parent) {
    parent->addWidget(this);
}
void CSplitter::moveSplitter(int pos, int index){
    if(index >= sizes().size()) return;
    bool oldState = blockSignals(true);
    // std::cout << " " << pos << std::endl;
    // pos -= 5;
    QSplitter::moveSplitter(pos, index);
    blockSignals(oldState);
}





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
