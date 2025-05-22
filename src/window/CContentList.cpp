#include "mainWidgets.h"

#include "../window/modlist/cobject.h"
#include <QLabel>
#include <QTimer>

CContentList::CContentList () {
    setMinimumWidth(200);
    QVBoxLayout* BaseContainer = new QVBoxLayout();
    setLayout(BaseContainer);

    QFrame* siFrame = new QFrame; // subinfo Frame
    QHBoxLayout* splitterBox = new QHBoxLayout(siFrame);
    BaseContainer->addWidget(siFrame);
    siFrame->setStyleSheet("background-color: #444b50; border-radius: 10px;");

    spl1 = new QSplitter;
    spl2 = new QSplitter;
    QLabel* lblName     = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_NAME"]));
    QLabel* lblVersion  = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"]));
    QLabel* lblType     = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_TYPE"]));
    QLabel* lblSwitcher = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_SWITCHER"]));

    splitterBox->addWidget(spl1);
    splitterBox->addWidget(spl2);
    splitterBox->addWidget(lblSwitcher);
    lblName->resize(400, 0);
    spl1->addWidget(lblName);
    spl1->addWidget(spl2);
    spl2->addWidget(lblVersion);
    spl2->addWidget(lblType);

    contentList = new CObjectsContainer;
    BaseContainer->addWidget(contentList);
    dnd = new CDND(BaseContainer, Core::lang["LANG_LABEL_DND"]);
}


void CContentList::updateList (CObjectsButton* pointer, bool type) {
    targetName = pointer->name;
    targetType = type;
    contentList->clear();
    if (type) sPath = stc::cwmm::ram_preset(targetName);
    else      sPath = stc::cwmm::ram_collection(targetName);
    wmml file(sPath);
    std::vector<wmml::variant> v(file.width());
    bool counter = false;
    for (unsigned long index = 0; file.read(v); ++index) {
        CObject* buttonWidget = new CObject(v, counter, index);
        contentList->add(buttonWidget);

        // Crutch. It will need to be fixed
        double sz11 = spl1->size().width();
        double sz21 = spl1->size().height();
        double sz31 = sz11 / (sz21 / 13);
        double sz12 = spl2->size().width();
        double sz22 = spl2->size().height();
        double sz32 = sz12 / (sz22 / 10.5);

        QTimer::singleShot(0, buttonWidget, [buttonWidget, sz31, sz32]() {
            buttonWidget->spl1->moveSplitter(sz31, 1);
            buttonWidget->spl2->moveSplitter(sz32, 1);
        });
        //

        connect(spl1, &QSplitter::splitterMoved, buttonWidget->spl1, &CSplitter::moveSplitter);
        connect(spl2, &QSplitter::splitterMoved, buttonWidget->spl2, &CSplitter::moveSplitter);
        connect(buttonWidget, &CObject::ON,     this, &CContentList::changeStatusOn);
        connect(buttonWidget, &CObject::OFF,    this, &CContentList::changeStatusOff);
        connect(buttonWidget, &CObject::remove, this, &CContentList::deleting);
    }
}

void CContentList::clear () {
    contentList->clear();
}

void CContentList::changeStatusOn(CObject* toggledElements) {
    wmml file(sPath);
    auto bol = true;
    file.overwriting_sector(toggledElements->index, 4, bol);
}
void CContentList::changeStatusOff(CObject* toggledElements) {
    wmml file(sPath);
    auto bol = false;
    file.overwriting_sector(toggledElements->index, 4, bol);
}
void CContentList::deleting (CObject* pointer) {
    wmml file(sPath);
    file.remove_object(pointer->index);
    delete pointer;
}
