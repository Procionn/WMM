#include "widgetpattern.h"
#include "lang.h"
#include "CONSTANTS.h"
#include "core.h"
#include "core.h"
#include "dnd.h"
#include "patterns\CLinkTumbler.h"
#include "wmml.h"
#include "methods.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>

CObjectList::CObjectList () {
    setMaximumWidth(1000);
    setMinimumWidth(230);
    QVBoxLayout* privateObjectList = new QVBoxLayout;
    setLayout(privateObjectList);
    QHBoxLayout* objectButtonBox = new QHBoxLayout();
    privateObjectList->addLayout(objectButtonBox);
    CLinkTumbler* Collection = new CLinkTumbler(Lang::LANG_BUTTON_COLLECTIONS);
    CLinkTumbler* Preset     = new CLinkTumbler(Lang::LANG_BUTTON_PRESETS, Collection);
    objectButtonBox->addWidget(Collection);
    objectButtonBox->addWidget(Preset);
    Collection->isTarget(true);
    Collection->setMinimumHeight(35);
    Preset->setMinimumHeight(35);
    
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    privateObjectList->addWidget(line);
    
    objectList = new QVBoxLayout();
    addScrollable(privateObjectList, objectList);
    privateObjectList->setAlignment(Qt::AlignTop);
    objectList->setAlignment(Qt::AlignTop);
    
    connect(Collection, &QPushButton::clicked, this, [=]{
        TypeTarget = false;
        render();
    });
    
    connect(Preset,     &QPushButton::clicked, this, [=]{
        TypeTarget = true;
        render();
    });
    updateList();
    render();
}

void CObjectList::newObject (CNewObjectDialog* dialog) {
    QString tab = dialog->nameTab->text();
    CreteObject(tab.toStdString());
    dialog->reject();
}


void CObjectList::CreteObject (std::string name) {
    std::filesystem::path path;
    if (TypeTarget) path = stc::cwmm::ram_preset(name);
    else            path = stc::cwmm::ram_collection(name);
    if (!std::filesystem::exists(path)) {
        stc::wmm::new_object(path);
        updateList();
        render();
    }
    else ERRORdialog* errorR34 = new ERRORdialog(Lang::LANG_LABEL_R34);
}



void CObjectList::scan_directory (const std::filesystem::path& directory, const bool type, CObjectsButton*& lastTumbler) {
    if (std::filesystem::exists(directory)) {
        for (auto const& object : std::filesystem::directory_iterator(directory)) {
            std::string newButton = stc::string::get_name(object.path().string());
            CObjectsButton* button = new CObjectsButton(newButton, lastTumbler);
            button->setMinimumHeight(35);
            button->SetLeftAlignment(true);
            button->type = type;
            objectList->addWidget(button);
            connect(button, &QPushButton::clicked, this, [=]{
                emit objectChoosed(button, TypeTarget);
                targetName = newButton;
            });
            connect(button, &CObjectsButton::remove, this, [=]{
                updateList();
                render();
                emit remove();
            });
            button->hide();
            list.emplace_back(button);
            lastTumbler = button;
        }
    }
}


void CObjectList::updateList (std::string toggledButton) {
    std::filesystem::path directory1 = stc::cwmm::ram_preset();
    std::filesystem::path directory2 = stc::cwmm::ram_collection();
    int counter = 0;
    CObjectsButton* lastTumbler = nullptr;
    for (const auto& entry : list)
        delete entry;
    list.clear();

    scan_directory(directory1, true, lastTumbler);
    scan_directory(directory2, false, lastTumbler);
}

void CObjectList::render() {
    if (TypeTarget) {
        for (CObjectsButton* target : list)
            if  (target->type == true) target->show();
            else target->hide();
    }
    else {
        for (CObjectsButton* target : list)
            if  (target->type == false) target->show();
            else target->hide();
    }
}





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
    QLabel* lblName     = new QLabel(QString::fromStdString(Lang::LANG_LABEL_NAME));
    QLabel* lblVersion  = new QLabel(QString::fromStdString(Lang::LANG_LABEL_VERSION));
    QLabel* lblType     = new QLabel(QString::fromStdString(Lang::LANG_LABEL_TYPE));
    QLabel* lblSwitcher = new QLabel(QString::fromStdString(Lang::LANG_LABEL_SWITCHER));
    
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
    dnd = new CDND(BaseContainer, Lang::LANG_LABEL_DND);
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
