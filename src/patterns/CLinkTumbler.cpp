#include "CLinkTumbler.h"

CLinkTumbler::CLinkTumbler(std::string name, CLinkTumbler* linked, QWidget* parent) : CToggledButton(parent, name){
    this->name = name;
    if (linked != nullptr) {
        link = linked;
        connect(this, &CLinkTumbler::clicked, linked, &CLinkTumbler::reset);
        connect(linked, &CLinkTumbler::clicked, this, &CLinkTumbler::reset);
        while (linked->link != nullptr) {
            linked = linked->link;
            connect(this, &CLinkTumbler::clicked, linked, &CLinkTumbler::reset);
            connect(linked, &CLinkTumbler::clicked, this, &CLinkTumbler::reset);
        }
    }
}

void CLinkTumbler::ChangeOfState() {
    if (!selected) {
        emit toggled(this, name);
        selected = true;
        setStyleSheet(QString::fromStdString(toggledColor));
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + toggledColor));
        else setStyleSheet(QString::fromStdString(toggledColor));
    }
}
void CLinkTumbler::reset() {
    if (selected) {
        selected = false;
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + untoggledColor));
        else setStyleSheet(QString::fromStdString(untoggledColor));
    }
}
