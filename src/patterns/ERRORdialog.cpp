#include "ERRORdialog.h"

ERRORdialog::ERRORdialog (std::string label) {
    show();
    apply->hide();
    QLayout* tempLay = new QVBoxLayout(list);
    tempLay->setAlignment(Qt::AlignTop);
    QLabel* lbl = new QLabel(QString::fromStdString(label));
    lbl->setWordWrap(true);
    tempLay->addWidget(lbl);
}
