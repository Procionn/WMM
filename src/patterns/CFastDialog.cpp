#include "CFastDialog.h"

CFastDialog::CFastDialog () {
    setMinimumHeight(230);
    setMinimumWidth(350);
    // setModal(Qt::WindowModal);
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    QHBoxLayout* DialogButtonBox = new QHBoxLayout();
    apply = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_APPLY"]), this);
    QPushButton* cansel = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_CANSEL"]), this);
    list = new QWidget();
    list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dialogLayout->addWidget(list);
    dialogLayout->addLayout(DialogButtonBox);
    DialogButtonBox->addWidget(apply);
    DialogButtonBox->addWidget(cansel);
    apply->setDefault(true);
    connect(apply,  &QPushButton::clicked, [=]{emit applyClicked();});
    connect(cansel, &QPushButton::clicked, [=]{
        reject();
        emit canselClicked();
    });
}
