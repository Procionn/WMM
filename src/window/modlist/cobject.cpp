#include "cobject.h"

CObject::CObject(const std::vector<std::string>& v, bool& counter) {
    Box = new QHBoxLayout(this);
    Lname = new QLabel(QString::fromStdString(v[1]));
    Lversion = new QLabel(QString::fromStdString(v[2]));
    Ltype = new QLabel(QString::fromStdString(v[3]));
    switcher = new CSwitchButton;


    index = std::stoi(v[0]);
    id = v[4];
    name = v[1];
    type = v[3];
    version = v[2];
    switcher->setTheme("orange");
    if (v[5] == "1") switcher->isTarget(true);
    else             switcher->isTarget(false);

    Lname->resize(400, 0);
    // setFrameShape(QFrame::Panel);
    // setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);
    // or use it ->
    // setFrameShadow(QFrame::Raised);
    spl1 = new CSplitter(Box);
    spl2 = new CSplitter(Box);
    spl1->addWidget(Lname);
    spl1->addWidget(spl2);
    spl2->addWidget(Lversion);
    spl2->addWidget(Ltype);
    Box->addWidget(switcher);

    connect(switcher, &CSwitchButton::toggled,   this, [=]{emit ON(this);});
    connect(switcher, &CSwitchButton::untoggled, this, [=]{emit OFF(this);});
    if (counter) {
        setStyleSheet(QString::fromStdString(untoggledColor1));
        counter = false;
    }
    else {
        setStyleSheet(QString::fromStdString(untoggledColor2));
        counter = true;
    }
    count_type = counter;
    // setStyleSheet("background-color: #444b52; border-radius: 10px;");
}


void CObject::context (const QPoint &pos) {
    QMenu* contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_DELETE));
    connect(action1, &QAction::triggered, this, &CObject::DELETE);
    if (type == "mod") {
        QAction *action2 = contextMenu->addAction(QString::fromStdString(Lang::LANG_BUTTON_INFO));
        connect(action2, &QAction::triggered, this, &CObject::INFO);
    }
    contextMenu->exec(this->mapToGlobal(pos));
}

void CObject::DELETE() {
    emit remove(this);
}

void CObject::INFO() {
    std::ifstream file(stc::cwmm::ram_mods_info(name));
    std::string id;
    for (int i = 0; i != 3; ++i)
        std::getline(file, id);
    stc::net::openURL(stc::cwmm::modsURL(id));
}

void CObject::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(untoggledColor1));
        else
            setStyleSheet(QString::fromStdString(untoggledColor2));
    }
}

void CObject::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(toggledColor1));
        else
            setStyleSheet(QString::fromStdString(toggledColor2));
    }
}




CSplitter::CSplitter(QHBoxLayout* parent) {
    parent->addWidget(this);
}
void CSplitter::moveSplitter(int pos, int index){
    if(index >= sizes().size()) return;
    bool oldState = blockSignals(true);
    QSplitter::moveSplitter(pos, index);
    blockSignals(oldState);
}
