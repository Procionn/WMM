#include "settings.h"
#include "../CONSTANTS.h"
#include "../patterns.h"
#include "../lang.h"
#include "../dialogs.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>


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
    sobjects->sorce->isTarget(true);
    
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
    if (!slist->ss->buffer.isEmpty())
        config.game_path(slist->ss->buffer.toStdString());
    CConfigs core;
    if (slist->ss->target != nullptr) {
        CConfigs::CONFIG_GAME = slist->ss->target->name;
        core.config_save();
    }
}





SObjects::SObjects () {
    list = new QVBoxLayout;
    CScrollWindow* scrollWindow = new CScrollWindow(this, list);
    sorce = new CLinkTumbler(Lang::LANG_BUTTON_SORCE);
    lang = new CLinkTumbler(Lang::LANG_BUTTON_LANG, sorce);
    support = new CLinkTumbler(Lang::LANG_BUTTON_SUPPORT, lang);
    extensions = new CLinkTumbler(Lang::LANG_BUTTON_EXTENSIONS, support);
    
    list->setAlignment(Qt::AlignTop);
    sorce->SetLeftAlignment(true);
    lang->SetLeftAlignment(true);
    support->SetLeftAlignment(true);
    extensions->SetLeftAlignment(true);
    
    // lang->hide();
    // support->hide();
    extensions->hide();
    
    sorce->setMinimumHeight(35);
    lang->setMinimumHeight(35);
    support->setMinimumHeight(35);
    extensions->setMinimumHeight(35);
    
    list->addWidget(sorce);
    list->addWidget(lang);
    list->addWidget(support);
    list->addWidget(extensions);
}





SList::SList () {
    QVBoxLayout* list = new QVBoxLayout(this);
    ss = new setsource;
    sl = new setlang;
    sup = new setsupport;
    se = new setextensions;
    list->addWidget(ss);
    list->addWidget(sl);
    list->addWidget(sup);
    list->addWidget(se);
}

void SList::sorce () {
    ss->show();
    sl->hide();
    sup->hide();
    se->hide();
}

void SList::lang () {
    ss->hide();
    sl->show();
    sup->hide();
    se->hide();
}

void SList::support () {
    ss->hide();
    sl->hide();
    sup->show();
    se->hide();
}

void SList::extensions () {
    ss->hide();
    sl->hide();
    sup->hide();
    se->show();
}
