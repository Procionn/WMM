#include "CMenuBar.h"

#include "../window.h"

CMenuBar::CMenuBar(Window* parent) {
    this->parent = parent;

    QAction* settings       = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_SETTINGS"]), this);
    QAction* object         = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_NEW_OBJECT"]), this);
    QAction* assembling     = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_ASSEMBLING"]), this);
    QAction* reassembly     = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_REASSEMBLY"]), this);
    QAction* linking        = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_LINKING"]), this);
    QAction* content        = new QAction(QString::fromStdString(Core::lang["LANG_BUTTON_ADD_COMPONENTS"]), this);

    addAction(settings);
    addAction(object);
    addAction(content);
    addAction(assembling);
    addAction(reassembly);
    addAction(linking);

    connect(content,    &QAction::triggered, [=]{emit content_click();});
    connect(settings,   &QAction::triggered, [=]{emit settings_click();});
    connect(object,     &QAction::triggered, [=]{emit object_click();});
    connect(assembling, &QAction::triggered, this, &CMenuBar::assembling_clicked);
    connect(reassembly, &QAction::triggered, this, &CMenuBar::reassembly_clicked);
    connect(linking,    &QAction::triggered, this, &CMenuBar::linking_clicked);
}



void CMenuBar::assembling_clicked () {
    if (parent->target != nullptr) {
        if (parent->ObjectList->TypeTarget)
            ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
        else  Core::get().collector(parent->target->name, parent->ObjectList->TypeTarget);
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R31"]);
}



void CMenuBar::linking_clicked () {
    if (parent->target != nullptr) {
        if (parent->ObjectList->TypeTarget)
            ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
        else {
            std::filesystem::remove_all(COLLECTIONS + Core::CONFIG_GAME + "/" + parent->target->name);
            Core::get().collector(parent->target->name, parent->ObjectList->TypeTarget);
        }
    }
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R31"]);
}



void CMenuBar::reassembly_clicked () {
    if (parent->target != nullptr && !parent->ObjectList->TypeTarget) {
        if (Core::CONFIG_GAME_PATH != "") {
            Core::get().symlink_deliting();
            Core::get().symlink_creating(parent->target->name);
        }
    }
    else if (parent->ObjectList->TypeTarget)
         ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R30"]);
    else ERRORdialog* dialog = new ERRORdialog(Core::lang["LANG_LABEL_R36"]);
}
