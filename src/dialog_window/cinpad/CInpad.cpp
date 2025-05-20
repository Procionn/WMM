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
    connect(presets, &QAction::triggered, this, [=]{
        menu->setTitle(QString::fromStdString(Lang::LANG_BUTTON_PRESETS));
        targetType = true;
        distributor();
    });

    QAction* mods = new QAction(QString::fromStdString(Lang::LANG_BUTTON_MODS));
    menu->addAction(mods);
    connect(mods, &QAction::triggered, this, [=]{
        menu->setTitle(QString::fromStdString(Lang::LANG_BUTTON_MODS));
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

bool CInpad::nameTest (std::vector<Cbox>& existsElements, int indicator, std::string str) {
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
    std::vector<wmml::variant> v(file.width());
    std::vector<Cbox> existsElements(file.height());
    int arraySize = 0;
    while (file.read(v)) {
        if (std::get<bool>(v[2]))
             existsElements[arraySize].path = stc::cwmm::ram_mods(std::get<std::string>(v[0]));
        else existsElements[arraySize].path = stc::cwmm::ram_preset(std::get<std::string>(v[0]));
        arraySize++;
    }
    bool count_type = false;
    fsScaner(stringDir1, true, arraySize, existsElements, count_type);
    fsScaner(stringDir2, false, arraySize, existsElements, count_type);
    vector = true;
}


void CInpad::fsScaner(const std::filesystem::path& directory, const bool& type,
                      const int& arraySize, std::vector<Cbox>& existsElements, bool& count_type) {
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
                    button = new CInpadButton(newButton, true, count_type);
                }
                else
                    button = new CInpadButton(newButton, false, count_type);
                newObjectList->add(button);
                button->setMinimumHeight(35);
                vlist.emplace_back(button);
                button->hide();
            }
        }
    }
}


void CInpad::application(std::string& targetName, bool targetType) {
    std::string name;
    if (targetType) name = stc::cwmm::ram_preset(targetName);
    else            name = stc::cwmm::ram_collection(targetName);
    wmml file(name);
    std::vector<wmml::variant> v(file.width());
    for (CInpadButton* target : vlist) {
        if (target->is_target()) {
            if  (target->type == true) {
                v = {target->get_name(), "", false, 0, true};
                file.write(v);
            }
            else {
                std::ifstream openedFile(stc::cwmm::ram_mods_info(target->get_name()));
                std::string str;
                for (int n = 0; n != 2; ++n)
                    std::getline(openedFile, str);
                std::string version = str;
                std::getline(openedFile, str);
                unsigned long id = std::stoi(str);
                v = {target->get_name(), version, true, id, true};
                file.write(v);
            }
        }
    }
    reset();
    this->reject();
}
