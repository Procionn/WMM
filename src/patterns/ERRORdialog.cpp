#include "ERRORdialog.h"

#include "../core.h"

ERRORdialog::ERRORdialog (const std::string& label) {
    show();
    apply->hide();
    QLayout* tempLay = new QVBoxLayout(list);
    tempLay->setAlignment(Qt::AlignTop);
    QLabel* lbl = new QLabel(QString::fromStdString(label));
    lbl->setWordWrap(true);
    tempLay->addWidget(lbl);
}





FatalError::FatalError (const std::string& error, bool fatality) {
    this->fatality = fatality;
    show();
    apply->hide();

    QVBoxLayout* tempLay = new QVBoxLayout(list);
    QLabel* label = new QLabel(QString::fromStdString(error));
    QLabel* errorLable = new QLabel("Fatal Error!");
    QPushButton* resetButton = new QPushButton(QString::fromStdString(Core::lang["LANG_BUTTON_RESET"]));

    if (fatality)
        tempLay->addWidget(errorLable);
    else
        delete errorLable;
    tempLay->setAlignment(Qt::AlignTop);
    label->setWordWrap(true);
    tempLay->addWidget(label);

    DialogButtonBox->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, [=]{
        throw new reset;
    });
}


FatalError::~FatalError () {
    if (fatality)
        abort();
}
