#include "setlang.h"
#include "../core.h"
#include "../CONSTANTS.h"
#include "../methods.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <filesystem>

setlang::setlang () {
    // std::string strlang = Core::CONFIG_LANGUAGES.substr(Core::CONFIG_LANGUAGES.find_last_of('/') + 1);

    QVBoxLayout* list = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;
    QLabel* lang = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CHOOSE_LANG"]));
    QLabel* community = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CUSTOM_LANG"]).replace("\\n", "\n"));
    QPushButton* button = new QPushButton(QString::fromStdString(stc::string::get_name(Core::CONFIG_LANGUAGES)));
    addScrollable(this, list);

    list->setAlignment(Qt::AlignTop);
    hbox->setAlignment(Qt::AlignLeft);
    community->setWordWrap(true);

    list->addLayout(hbox);
    hbox->addWidget(lang);
    hbox->addWidget(button);
    list->addWidget(community);

    connect(button, &QPushButton::clicked, [=]{chooseLang(button);});
}


void setlang::chooseLang (QPushButton* parent) {
    CFastDialog* chooser = new CFastDialog;
    QVBoxLayout* list = new QVBoxLayout;
    addScrollable(chooser->list, list);

    chooser->show();
    list->setAlignment(Qt::AlignTop);

    CLinkTumbler* lastBTN = nullptr;
    for (const auto& entry : std::filesystem::directory_iterator(LANG)) {
        std::string lang = entry.path().generic_string().substr(0, entry.path().generic_string().size() - INI_MAIN_PART);
        size_t part = lang.find_last_of('/');
        lang = lang.substr(part + 1);
        CLinkTumbler* button = new CLinkTumbler(lang, lastBTN);
        lastBTN = button;
        list->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [=]{target = button;});
        connect(chooser->apply, &QPushButton::clicked, [=]{parent->setText(QString::fromStdString(target->name));
                                                           chooser->reject();
                                                          });
    }
}
