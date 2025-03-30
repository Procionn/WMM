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

    newObjectList = new CInpadList;
    vertBox->addWidget(newObjectList);
}

void CInpad::reset() {
    newObjectList->clear();
    vector = false;
    vlist.clear();
    menu->setTitle("v");
    if (!(*callType)) presets->setVisible(true);
    else presets->setVisible(false);
}

void CInpad::distributor() {
    if (!vector)
        reader();
    render();
}

void CInpad::render () {
    if (targetType) {
        for (CInpadButton* target : vlist)
            if  (target->type == true)
                target->show();
            else target->hide();
    }
    else {
        for (CInpadButton* target : vlist)
            if  (target->type == false)
                target->show();
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
                CInpadButton* button;
                if (type) {
                    newButton = newButton.substr(0, newButton.size() - MAIN_PART);
                    button = new CInpadButton(newButton, true);
                }
                else
                    button = new CInpadButton(newButton, false);
                newObjectList->add(button);
                button->setMinimumHeight(35);
                vlist.emplace_back(button);
                button->hide();
            }
        }
    }
}


void CInpad::application(std::string targetName, bool targetType) {
    std::string name;
    if (targetType) name = stc::cwmm::ram_preset(targetName);
    else            name = stc::cwmm::ram_collection(targetName);
    std::vector<std::string> v(GRID_WIDTH);
    wmml file(name);
    while (file.read(v));
    int i;
    if (v[0] != "") {
        i = std::stoi(v[0]);
        ++i;
    }
    else i = 0;
    for (CInpadButton* target : vlist) {
        if (target->is_target()) {
            if  (target->type == true) {
                v[0] = std::to_string(i);
                v[1] = target->get_name();
                v[2] = "None";
                v[3] = "preset";
                v[4] = "None";
                v[5] = "1";
                file.add(v);
            }
            else if (target->type == false) {
                std::ifstream openedFile(stc::cwmm::ram_mods_info(target->get_name()));
                std::string str;
                for (int n = 0; n != 2; ++n)
                    std::getline(openedFile, str);
                v[2] = str;
                std::getline(openedFile, str);
                v[4] = str;
                v[0] = std::to_string(i);
                v[1] = target->get_name();
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
