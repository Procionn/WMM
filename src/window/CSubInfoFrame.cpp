#include "CSubInfoFrame.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "../core.h"

CSubInfoFrame::CSubInfoFrame (QVBoxLayout* parent) {
    QScrollArea* scroll = new QScrollArea;
    QWidget* scrolledWidget = new QWidget;
    scroll->setWidgetResizable(true);
    scroll->setWidget(scrolledWidget);
    scrolledWidget->setLayout(this);
    parent->addWidget(scroll);

    QFrame* siFrame = new QFrame(); // SubInfo Frame
    addWidget(siFrame);

    QHBoxLayout* splitterBox = new QHBoxLayout(siFrame);
    QLabel* lblName = new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_NAME"]));
    lblName->resize(400, 0);

    spl1 = new QSplitter;
    spl2 = new QSplitter;

    splitterBox->addWidget(spl1);
    spl1->addWidget(lblName);
    spl1->addWidget(spl2);
    spl2->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"])));
    spl2->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_TYPE"])));
    splitterBox->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_SWITCHER"])));

    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    siFrame->setStyleSheet("background-color: #444b50; border-radius: 10px;");
}
