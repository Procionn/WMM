#include "CToggledButton.h"

CToggledButton::CToggledButton(std::string setName, bool type) {
    name = setName;
    selected = type;
    setText(QString::fromStdString(setName));
    connect(this, &QPushButton::clicked, this, &CToggledButton::ChangeOfState);
}
CToggledButton::CToggledButton(QWidget* parent, std::string setName, bool type) : QPushButton(parent) {
    name = setName;
    selected = type;
    setText(QString::fromStdString(setName));
    connect(this, &QPushButton::clicked, this, &CToggledButton::ChangeOfState);
}
#if 0
CToggledButton::CToggledButton(QVBoxLayout* parent, std::string setName, bool type){
    parent->addWidget(this);
    name = setName;
    selected = type;
    setText(QString::fromStdString(setName));
    connect(this, &QPushButton::clicked, this, &CToggledButton::ChangeOfState);
}
CToggledButton::CToggledButton(QHBoxLayout* parent, std::string setName, bool type){
    parent->addWidget(this);
    name = setName;
    selected = type;
    setText(QString::fromStdString(setName));
    connect(this, &QPushButton::clicked, this, &CToggledButton::ChangeOfState);
}
#endif
void CToggledButton::SetLeftAlignment(bool type) {
    if (type) {
        setStyleSheet("text-align: left; padding-left: 10px; background-color: #31363b;");
        leftAlignment = true;
    }
}

void CToggledButton::ChangeOfState() {
    if (selected) {
        emit untoggled(this, name);
        this->setText(QString::fromStdString(name));
        selected = false;
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + untoggledColor));
        else setStyleSheet(QString::fromStdString(untoggledColor));
    }
    else {
        emit toggled(this, name);
        this->setText(QString::fromStdString(TBC1 + name + TBC2));
        selected = true;
        if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + toggledColor));
        else setStyleSheet(QString::fromStdString(toggledColor));
    }
}

void CToggledButton::setTheme(std::string type) {
    if (type == "base") {
        untoggledColor = " background-color: #31363b;";
        toggledColor = " background-color: #444b52;";
    }
    else if (type == "orange") {
        toggledColor = " background-color: #d67418";
    }
    else if (type == "red") {
        toggledColor = " background-color: #c22124";
    }
    if (leftAlignment) setStyleSheet(QString::fromStdString(leftAlign + untoggledColor));
    else setStyleSheet(QString::fromStdString(untoggledColor));
}

void CToggledButton::setColor(std::string toggled, std::string untoggled){
    untoggledColor = " background-color: " + untoggled + ";";
    toggledColor   = " background-color: " + toggled   + ";";
}
void CToggledButton::isTarget(bool type) {
    if (type)
        ChangeOfState();
}
