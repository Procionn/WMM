#include "setsupport.h"

#include <QVBoxLayout>
#include "../core.h"

setsupport::setsupport() {
    QVBoxLayout* list = new QVBoxLayout(this);
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_WMM"])));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_VERSION"] + VERSION)));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_USED_LIBS"])));
    list->addWidget(new QLabel("\tQt Framework"));
    list->addWidget(new QLabel("\tLibarchive"));
    list->addWidget(new QLabel("\tWMML"));
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LANG_LABEL_CREDITS"])));
    list->addWidget(new QLabel("\tQt Framework\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_QT"])));
    list->addWidget(new QLabel("\tLibarchive\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_LIBARCHIVE"])));
    list->addWidget(new QLabel("\tJekkks\t\t" + QString::fromStdString(Core::lang["LABEL_CREDITS_JEKKKS"])));
    list->addWidget(new QLabel());
    list->addWidget(new QLabel(QString::fromStdString(Core::lang["LABEL_CREDITS_MORE_INFO"])));

    list->setAlignment(Qt::AlignTop);
}

