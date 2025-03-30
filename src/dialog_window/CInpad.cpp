#include "CInpad.h"

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
    addScrollable(vertBox, newObjectList);
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
    for (int i = 0; i != indicator; ++i)
        if (str == existsElements[i].path)
            return false;
    return true;
}

void CInpad::reader () {
    std::string stringDir1 = stc::cwmm::ram_preset();
    std::string stringDir2 = stc::cwmm::ram_mods();
    int counter = 0;
    std::string targetFiledDirectory;
    if ((*target)->type) targetFiledDirectory = stc::cwmm::ram_preset((*target)->name);
    else                 targetFiledDirectory = stc::cwmm::ram_collection((*target)->name);

    wmml file(targetFiledDirectory);
    std::vector<std::string> v(GRID_WIDTH);
    Cbox existsElements[file.sizeRequest()];
    file.reset();
    int arraySize = 0;
    while (file.read(v)) {
        existsElements[arraySize].index = std::stoi(v[0]);
        if      (v[3] == "mod")    existsElements[arraySize].path = stc::cwmm::ram_mods(v[1]);
        else if (v[3] == "preset") existsElements[arraySize].path = stc::cwmm::ram_preset(v[1]);
        else {
            std::cerr << "file is corrupted" << std::endl;
            abort();
        }
        arraySize++;
    }
    fsScaner(stringDir1, true, arraySize, existsElements);
    fsScaner(stringDir2, false, arraySize, existsElements);
    vector = true;
}


void CInpad::fsScaner(const std::filesystem::path& directory, const bool& type,
                      const int& arraySize, Cbox existsElements[]) {
    if (std::filesystem::exists(directory)) {
        for (auto const& objects : std::filesystem::directory_iterator{directory}) {
            std::string newButton = objects.path().string();
            stc::string::replace(newButton, '\\', '/');
            if (nameTest(existsElements, arraySize, newButton)) {
                size_t part = newButton.find_last_of('/');
                newButton = newButton.substr(part + 1);
                CInpadsButton* button;
                if (type) {
                    newButton = newButton.substr(0, newButton.size() - MAIN_PART);
                    button = new CInpadsButton(newObjectList, newButton, true);
                }
                else
                    button = new CInpadsButton(newObjectList, newButton, false);
                button->setMinimumHeight(35);
                button->SetLeftAlignment(true);
                vlist.emplace_back(button);
                button->hide();
            }
        }
    }
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
