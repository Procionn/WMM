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
    
    connect(Collection, &QPushButton::clicked, this, [=]{TypeTarget = false;
                                                        render();
                                                        });
    
    connect(Preset,     &QPushButton::clicked, this, [=]{TypeTarget = true;
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
    std::string path;
    if (TypeTarget) path = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS     + name + EXPANSION;
    else            path = RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS + name + EXPANSION;
    if (!std::filesystem::exists(path)) {
        std::filesystem::path dir = path;
        std::filesystem::create_directories(dir.parent_path());
        wmml file(path, GRID_WIDTH);
        if (!std::filesystem::exists(RAM)) std::filesystem::create_directory(RAM);
        updateList();
        render();
    }
    else ERRORdialog* errorR34 = new ERRORdialog(Lang::LANG_LABEL_R34);
}

void CObjectList::updateList (std::string toggledButton) {
    std::string stringDir1 = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS;
    std::string stringDir2 = RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS;
    std::filesystem::path directory1 = stringDir1;
    std::filesystem::path directory2 = stringDir2;
    int counter = 0;
    CObjectsButton* lastTumbler = nullptr;
    QLayoutItem* item;
    while ((item = objectList->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (std::filesystem::exists(directory1))
        for (auto const& objects : std::filesystem::directory_iterator{directory1})
            ++counter;
    if (std::filesystem::exists(directory2))
        for (auto const& objects : std::filesystem::directory_iterator{directory2})
            ++counter;
    list.resize(counter);
    counter = 0;
    if (std::filesystem::exists(directory1)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory1}) {
            std::string newButton = objects.path().string();
            size_t part = newButton.find_last_of('/');
            newButton = newButton.substr(part + 1);
            newButton = newButton.substr(0, newButton.size() - MAIN_PART);
            CObjectsButton* button = new CObjectsButton(newButton, lastTumbler);
            button->setMinimumHeight(35);
            button->SetLeftAlignment(true);
            button->type = true;
            objectList->addWidget(button);
            connect(button, &QPushButton::clicked, this, [=]{emit objectChoosed(button, TypeTarget);
                                                             targetName = newButton;
                                                            });
            connect(button, &CObjectsButton::remove, this, [=]{updateList();
                                                               render();
                                                               emit remove();
                                                              });
            button->hide();
            list[counter] = button;
            lastTumbler = button;
            ++counter;
        }
    }
    if (std::filesystem::exists(directory2)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory2}) {
            std::string newButton = objects.path().string();
            size_t part = newButton.find_last_of('/');
            newButton = newButton.substr(part + 1);
            newButton = newButton.substr(0, newButton.size() - MAIN_PART);
            CObjectsButton* button = new CObjectsButton(newButton, lastTumbler);
            button->setMinimumHeight(35);
            button->SetLeftAlignment(true);
            button->type = false;
            objectList->addWidget(button);
            connect(button, &QPushButton::clicked, this, [=]{emit objectChoosed(button, TypeTarget);
                                                             targetName = newButton;
                                                            });
            connect(button, &CObjectsButton::remove, this, [=]{updateList();
                                                               render();
                                                               emit remove();
                                                              });
            button->hide();
            list[counter] = button;
            lastTumbler = button;
            ++counter;
        }
    }
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
    std::vector<std::string> v(GRID_WIDTH);
    bool counter = false;
    while (file.read(v)) {
        CObject* buttonWidget = new CObject(v, counter);
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
    file.overwriting(toggledElements->index, 5, "1");
}
void CContentList::changeStatusOff(CObject* toggledElements) {
    wmml file(sPath);
    file.overwriting(toggledElements->index, 5, "0");
}
void CContentList::deleting (CObject* pointer) {
    // std::string path;
    // if (targetType) path = stc::cwmm::ram_preset(targetName);
    // else            path = stc::cwmm::ram_collection(targetName);
    wmml file(sPath);
    file.remove(pointer->index);
    delete pointer;
}
