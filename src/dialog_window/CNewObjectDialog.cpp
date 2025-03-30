#include "CNewObjectDialog.h"

CNewObjectDialog::CNewObjectDialog () {
    QHBoxLayout* glist = new QHBoxLayout;
    list->setLayout(glist);
    glist->setAlignment(Qt::AlignTop);
    glist->setContentsMargins(15, 15, 0, 0);
    glist->addWidget(new QLabel(QString::fromStdString(Lang::LANG_LABEL_NAME)));
    nameTab = new QLineEdit;
    glist->addWidget(nameTab);
}
