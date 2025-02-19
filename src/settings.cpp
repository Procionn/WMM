#include "settings.h"
#include "CONSTANTS.h"
#include "patterns.h"
#include "lang.h"
#include "dialogs.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <iostream>


CSettings::CSettings () {
    setMinimumHeight(200);
    setMinimumWidth(400);
    resize(1000, 600);
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    QHBoxLayout* dialogButtonBox = new QHBoxLayout();
    QPushButton* accept = new QPushButton(QString::fromStdString(Lang::LANG_BUTTON_ACCEPT));
    QPushButton* cansel = new QPushButton(QString::fromStdString(Lang::LANG_BUTTON_CANSEL));
    QWidget* separator = new QWidget;
    QSplitter* splitter = new QSplitter;
    QFrame* line = new QFrame();
    sobjects = new SObjects;
    slist = new SList;
    
    slist->resize(500, 10);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(cansel, &QPushButton::clicked, this, [=]{this->reject();});
    connect(accept, &QPushButton::clicked, this, &CSettings::save);
    connect(sobjects->sorce,   &CLinkTumbler::toggled, slist, &SList::sorce);
    connect(sobjects->lang,    &CLinkTumbler::toggled, slist, &SList::lang);
    connect(sobjects->support, &CLinkTumbler::toggled, slist, &SList::support);
    
    dialogLayout->addLayout(dialogButtonBox);
    dialogLayout->addWidget(line);
    dialogButtonBox->addWidget(accept);
    dialogButtonBox->addWidget(separator);
    dialogButtonBox->addWidget(cansel);
    dialogLayout->addWidget(splitter);
    splitter->addWidget(sobjects);
    splitter->addWidget(slist);
    show();
}

void CSettings::save () {
    CGameConfig config;
    CConfigs core;
    if (!slist->buffer.isEmpty())
        config.game_path(slist->buffer.toStdString());
    if (slist->target != nullptr) {
        CConfigs::CONFIG_GAME = slist->target->name;
        core.config_save();
    }
}





SObjects::SObjects () {
    list = new QVBoxLayout;
    CScrollWindow* scrollWindow = new CScrollWindow(this, list);
    sorce = new CLinkTumbler(Lang::LANG_BUTTON_SORCE);
    lang = new CLinkTumbler(Lang::LANG_BUTTON_LANG, sorce);
    support = new CLinkTumbler(Lang::LANG_BUTTON_SUPPORT, lang);
    
    list->setAlignment(Qt::AlignTop);
    sorce->SetLeftAlignment(true);
    lang->SetLeftAlignment(true);
    
    lang->hide();
    support->hide();
    
    support->SetLeftAlignment(true);
    sorce->setMinimumHeight(35);
    lang->setMinimumHeight(35);
    support->setMinimumHeight(35);
    
    list->addWidget(sorce);
    list->addWidget(lang);
    list->addWidget(support);
}





SList::SList () {
    QHBoxLayout* list = new QHBoxLayout;
    QFrame* line = new QFrame();
    firstlist = new QVBoxLayout;
    lastlist = new QVBoxLayout;
    QWidget* firstwidget = new QWidget;
    QWidget* lastwidget = new QWidget;
    CScrollWindow* scrollWindow = new CScrollWindow(this, list);
    QLabel* dir = new QLabel(QString::fromStdString(Lang::LANG_LABEL_DIRECTORY_CHOOSE));
    QLabel* game = new QLabel(QString::fromStdString(Lang::LANG_LABEL_GAME_CHOOSE));
    QLabel* backup = new QLabel(QString::fromStdString(Lang::LANG_LABEL_GAME_BACKUP));
    QPushButton* dirBTN = new QPushButton(QString::fromStdString(""));
    QPushButton* gameBTN = new QPushButton(QString::fromStdString(CConfigs::CONFIG_GAME));
    QPushButton* backupBTN = new QPushButton(QString::fromStdString(Lang::LANG_BUTTON_GAME_BACKUP));
    
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);    
    
    list->addWidget(firstwidget);
    list->addWidget(line);
    list->addWidget(lastwidget);
    firstwidget->setLayout(firstlist);
    lastwidget->setLayout(lastlist);
    list->setStretch(0, 0);
    list->setStretch(1, 0);
    list->setStretch(2, 1);
    
    firstlist->setAlignment(Qt::AlignTop);
    lastlist->setAlignment(Qt::AlignTop);
    firstlist->setSizeConstraint(QLayout::SetMinimumSize);
    firstlist->addWidget(dir);
    firstlist->addWidget(game);
    firstlist->addWidget(backup);
    dir->setMinimumHeight(35);
    game->setMinimumHeight(35);
    backup->setMinimumHeight(35);
    lastlist->addWidget(dirBTN);
    lastlist->addWidget(gameBTN);
    lastlist->addWidget(backupBTN);
    
    dirBTN->setText(QString::fromStdString(CConfigs::CONFIG_GAME_PATH));
    connect(dirBTN, &QPushButton::clicked, [=]{
        if (CConfigs::CONFIG_GAME != "None") {
            buffer = QFileDialog::getOpenFileName(
                nullptr,
                QString::fromStdString(Lang::LANG_LABEL_CHOOSE_GAME_FILE),
                "",
                QString::fromStdString(Lang::LANG_LABEL_ALL_FILE + " (*.exe)"));
            if (!buffer.isEmpty()) {
                dirBTN->setText(buffer);
            }
        }
        else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R33);
    });
    connect(backupBTN, &QPushButton::clicked, [=]{
        if (CConfigs::CONFIG_GAME != "None") {
            CGameConfig config;
            config.game_dir_backup();
        }
        else ERRORdialog* dialog = new ERRORdialog(Lang::LANG_LABEL_R32);
    });
    connect(gameBTN, &QPushButton::clicked, [=]{chooseLang(gameBTN);});
}

void SList::sorce () {
    
}

void SList::lang () {
    
}

void SList::support () {
    
}

void SList::chooseLang(QPushButton* parent) {
    CFastDialog* dialog = new CFastDialog;
    QVBoxLayout* content = new QVBoxLayout;
    CScrollWindow* scrollwindow = new CScrollWindow(dialog->list, content);
    
    dialog->show();
    content->setAlignment(Qt::AlignTop);
    CLinkTumbler* lastBTN = nullptr;
    for (const auto& entry : std::filesystem::directory_iterator(GAMES)) {
        std::string game = entry.path().generic_string().substr(0, entry.path().generic_string().size() - INI_MAIN_PART);
        size_t part = game.find_last_of('/');
        game = game.substr(part + 1);
        CLinkTumbler* button = new CLinkTumbler(game, lastBTN);
        lastBTN = button;
        content->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [=]{tmptarget = button;});
        connect(dialog->apply, &QPushButton::clicked, [=]{target = tmptarget; parent->setText(QString::fromStdString(game)); dialog->reject();});
    }
    
}
