#include "setsource.h"

#include <filesystem>
#include <QFileDialog>
#include "../methods.h"

setsource::setsource() {
    QHBoxLayout* list = new QHBoxLayout;
    QFrame* line = new QFrame();
    firstlist = new QVBoxLayout;
    lastlist = new QVBoxLayout;
    QWidget* firstwidget = new QWidget;
    QWidget* lastwidget = new QWidget;
    addScrollable(this, list);
    QLabel* dir                 = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_DIRECTORY_CHOOSE"]));
    QLabel* game                = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_CHOOSE"]));
    QLabel* backup              = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_BACKUP"]));
    QLabel* clear               = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_CLEAR"]));
    QLabel* recovery            = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_GAME_RECOVERY"]));
    QPushButton* dirBTN         = new QPushButton(QString::fromStdString(""));
    QPushButton* gameBTN        = new QPushButton(QString::fromStdString(Core::CONFIG_GAME));
    QPushButton* backupBTN      = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_BACKUP"]));
    QPushButton* clearBTN       = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_CLEAR"]));
    QPushButton* recoveryBTN    = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_GAME_RECOVERY"]));

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
    firstlist->addWidget(clear);
    firstlist->addWidget(recovery);
    dir->setMinimumHeight(35);
    game->setMinimumHeight(35);
    backup->setMinimumHeight(35);
    clear->setMinimumHeight(35);
    recovery->setMinimumHeight(35);
    dirBTN->setMinimumHeight(35);
    gameBTN->setMinimumHeight(35);
    backupBTN->setMinimumHeight(35);
    clearBTN->setMinimumHeight(35);
    recoveryBTN->setMinimumHeight(35);
    lastlist->addWidget(dirBTN);
    lastlist->addWidget(gameBTN);
    lastlist->addWidget(backupBTN);
    lastlist->addWidget(clearBTN);
    lastlist->addWidget(recoveryBTN);

    dirBTN->setText(QString::fromStdString(Core::CONFIG_GAME_PATH));
    connect(dirBTN, &QPushButton::clicked, [=]{chooseExe(dirBTN);});
    connect(backupBTN, &QPushButton::clicked, [=]{createBackup();});
    connect(gameBTN, &QPushButton::clicked, [=]{chooseGame(gameBTN);});
    connect(clearBTN, &QPushButton::clicked, [=]{Core::get().symlink_deliting();});
    connect(recoveryBTN, &QPushButton::clicked, [=]{Core::get().game_recovery();});
}



void setsource::chooseGame (QPushButton* parent) {
    CFastDialog* dialog = new CFastDialog;
    QVBoxLayout* content = new QVBoxLayout;
    addScrollable(dialog->list, content);

    dialog->show();
    content->setAlignment(Qt::AlignTop);
    CLinkTumbler* lastBTN = nullptr;
    for (const auto& entry : std::filesystem::directory_iterator(GAMES)) {
        CLinkTumbler* button = new CLinkTumbler(stc::string::get_name(entry.path().string()), lastBTN);
        lastBTN = button;
        content->addWidget(button);
        connect(button, &CLinkTumbler::toggled, [=]{tmptarget = button;});
        connect(dialog->apply, &QPushButton::clicked, [=]{
            target = tmptarget;
            parent->setText(QString::fromStdString(button->name));
            dialog->reject();
        });
    }
}


void setsource::chooseExe (QPushButton* dirBTN) {
    if (Core::CONFIG_GAME != "None" || target) {
        buffer = QFileDialog::getOpenFileName(
            nullptr,
            QString::fromStdString(Core::lang["LANG_LABEL_CHOOSE_GAME_FILE"]),
            "",
            QString::fromStdString(Core::lang["LANG_LABEL_ALL_FILE"] + " (*.exe)"));
        if (!buffer.isEmpty()) {
            dirBTN->setText(buffer);
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R33"]);
}


void setsource::createBackup () {
    if (Core::CONFIG_GAME != "None") {
        Core::get().game_dir_backup();
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R32"]);
}
