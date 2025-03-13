#include "dialogs.h"
#include "lang.h"
#include "patterns.h"
#include "CONSTANTS.h"
#include "core.h"
#include "wmml.h"
#include "methods.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QPushButton>

CFastDialog::CFastDialog () {
    setMinimumHeight(230);
    setMinimumWidth(350);
    // setModal(Qt::WindowModal);
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    QHBoxLayout* DialogButtonBox = new QHBoxLayout();
    apply = new QPushButton(QString::fromStdString(Lang::LANG_BUTTON_APPLY), this);
    QPushButton* cansel = new QPushButton(QString::fromStdString(Lang::LANG_BUTTON_CANSEL), this);
    list = new QWidget();
    list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dialogLayout->addWidget(list);
    dialogLayout->addLayout(DialogButtonBox);
    DialogButtonBox->addWidget(apply);
    DialogButtonBox->addWidget(cansel);
    connect(cansel, &QPushButton::clicked, this, [=]{reject(); emit canselClicked();});
    apply->setDefault(true);
    connect(apply, &QPushButton::clicked, this, [=]{emit applyClicked();});
}





ERRORdialog::ERRORdialog (std::string label) {
    show();
    apply->hide();
    QLayout* tempLay = new QVBoxLayout(list);
    tempLay->setAlignment(Qt::AlignTop);
    QLabel* lbl = new QLabel(QString::fromStdString(label));
    lbl->setWordWrap(true);
    tempLay->addWidget(lbl);
}





CNewObjectDialog::CNewObjectDialog () {
    QHBoxLayout* glist = new QHBoxLayout;
    list->setLayout(glist);
    glist->setAlignment(Qt::AlignTop);
    glist->setContentsMargins(15, 15, 0, 0);
    glist->addWidget(new QLabel(QString::fromStdString(Lang::LANG_LABEL_NAME)));
    nameTab = new QLineEdit;
    glist->addWidget(nameTab);
}





CInpad::CInpad(bool& type) {
    callType = &type;
    setMinimumHeight(400);
    setMinimumWidth(600);
    QVBoxLayout* vertBox = new QVBoxLayout(list);
    QHBoxLayout* horBox = new QHBoxLayout;
    vertBox->addLayout(horBox);
    QLabel* label = new QLabel(QString::fromStdString(Lang::LANG_LABEL_ADD));
    QMenuBar* menuBar = new QMenuBar;
    horBox->addWidget(label);
    horBox->addWidget(menuBar);
    
    menu = new QMenu("v");
    menuBar->addMenu(menu);
    
    presets = new QAction(QString::fromStdString(Lang::LANG_BUTTON_PRESETS));
    menu->addAction(presets);
    connect(presets, &QAction::triggered, this, [=]{menu->setTitle(QString::fromStdString(Lang::LANG_BUTTON_PRESETS));
                                                    targetType = true;
                                                    distributor();
                                                    });
    
    QAction* mods = new QAction(QString::fromStdString(Lang::LANG_BUTTON_MODS));
    menu->addAction(mods);
    connect(mods, &QAction::triggered, this, [=]{menu->setTitle(QString::fromStdString(Lang::LANG_BUTTON_MODS));
                                                 targetType = false;
                                                 distributor();
                                                 });
    
    newObjectList = new QVBoxLayout;
    newObjectList->setAlignment(Qt::AlignTop);
    CScrollWindow* scrollWindow = new CScrollWindow(vertBox, newObjectList);
}

void CInpad::reset() {
    QLayoutItem* item;
    while ((item = newObjectList->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    vector = false;
    menu->setTitle("v");
    if (!(*callType)) presets->setVisible(true);
    else presets->setVisible(false);
}

void CInpad::distributor() {
    if (!vector) reader();
    render();    
}

void CInpad::render () {
    if (targetType) {
        for (CInpadsButton* target : vlist)
            if  (target->type == true) target->show();
            else target->hide();
    }
    else {
        for (CInpadsButton* target : vlist)
            if  (target->type == false) target->show();
            else target->hide();
    }
}

bool CInpad::nameTest (Cbox existsElements[], int indicator, std::string str) {
    for (int i = 0; i != indicator; ++i) {
        // std::cout << str << "->" << existsElements[i].path << std::endl;
        if (str == existsElements[i].path) return false;
    }
    return true;
}

void CInpad::reader () {
    std::string stringDir1 = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS;
    std::string stringDir2 = MODS + CConfigs::CONFIG_GAME;
    std::filesystem::path directory1 = stringDir1;
    std::filesystem::path directory2 = stringDir2;
    int counter = 0;
    std::string targetFiledDirectory;
    if ((*target)->type) targetFiledDirectory = stringDir1 + (*target)->name + EXPANSION;
    else targetFiledDirectory = RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS + (*target)->name + EXPANSION;
    
    wmml file(targetFiledDirectory);
    std::vector<std::string> v(GRID_WIDTH);
    Cbox existsElements[file.sizeRequest()];
    file.reset();
    int arraySize = 0;
    while (file.read(v)) {
        existsElements[arraySize].index = std::stoi(v[0]);
        if      (v[3] == "mod")    existsElements[arraySize].path = MODS + CConfigs::CONFIG_GAME + "/" + v[1];
        else if (v[3] == "preset") existsElements[arraySize].path = RAM  + CConfigs::CONFIG_GAME + "/" + PRESETS + v[1] + EXPANSION;
        else {
            std::cerr << "file is corrupted" << std::endl;
            abort();
        }
        arraySize++;
    }
    
    if (std::filesystem::exists(directory1))
        for (auto const& objects : std::filesystem::directory_iterator{directory1})
            ++counter;
    if (std::filesystem::exists(directory2))
        for (auto const& objects : std::filesystem::directory_iterator{directory2})
            ++counter;
    vlist.resize(counter - arraySize);
    counter = 0;
    if (std::filesystem::exists(directory1)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory1}) {
            std::string newButton = objects.path().string();
            stc::string::replace(newButton, '\\', '/');
            if (nameTest(existsElements, arraySize, newButton)) {
                size_t part = newButton.find_last_of('/');
                newButton = newButton.substr(part + 1);
                newButton = newButton.substr(0, newButton.size() - MAIN_PART);
                CInpadsButton* button = new CInpadsButton(newObjectList, newButton, true);
                button->setMinimumHeight(35);
                button->SetLeftAlignment(true);
                vlist[counter] = button;
                button->hide();
                ++counter;
            }
        }
    }
    if (std::filesystem::exists(directory2)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory2}) {
            std::string newButton = objects.path().string();
            stc::string::replace(newButton, '\\', '/');
            if (nameTest(existsElements, arraySize, newButton)) {
                size_t part = newButton.find_last_of('/');
                newButton = newButton.substr(part + 1);
                CInpadsButton* button = new CInpadsButton(newObjectList, newButton, false);
                button->setMinimumHeight(35);
                button->SetLeftAlignment(true);
                vlist[counter] = button;
                button->hide();
                ++counter;
            }
        }
    }
    vector = true;
}


void CInpad::application(std::string targetName, bool targetType) {
    std::string name;
    if (targetType) name = RAM + CConfigs::CONFIG_GAME + "/" + PRESETS     + targetName + EXPANSION;
    else            name = RAM + CConfigs::CONFIG_GAME + "/" + COLLECTIONS + targetName + EXPANSION;
    std::vector<std::string> v(GRID_WIDTH);
    wmml file(name);
    while (file.read(v));
    int i;
    if (v[0] != "") {
        i = std::stoi(v[0]);
        ++i;
    }
    else i = 0;
    for (CInpadsButton* target : vlist) {
        if (target->selected) {
            if  (target->type == true) {
                v[0] = std::to_string(i);
                v[1] = target->name;
                v[2] = "None";
                v[3] = "preset";
                v[4] = "None";
                v[5] = "1";
                file.add(v);
            }
            else if (target->type == false) {
                std::ifstream openedFile(MODS + CConfigs::CONFIG_GAME + "/" + target->name + "/" + WML + target->name + EXPANSION2);
                std::string str;
                for (int n = 0; n != 2; ++n)
                    std::getline(openedFile, str);
                v[2] = str;
                std::getline(openedFile, str);
                v[4] = str;
                v[0] = std::to_string(i);
                v[1] = target->name;
                v[3] = "mod";
                v[5] = "1";
                file.add(v);
            }
            ++i;
        }
    }
    reset();
    this->reject();
}
