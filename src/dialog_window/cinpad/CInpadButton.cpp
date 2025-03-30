#include "CInpadButton.h"

CInpadButton::CInpadButton(const std::string& setName, const bool& type) {
    this->type = type;
    name = setName;
    QVBoxLayout* list = new QVBoxLayout(this);
    label = new QLabel(QString::fromStdString(setName));
    list->addWidget(label);
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
    setStyleSheet(QString::fromStdString(untoggledColor));
}


void CInpadButton::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        setStyleSheet(QString::fromStdString(untoggledColor));
    }
}

void CInpadButton::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        setStyleSheet(QString::fromStdString(toggledColor));
    }
}

bool CInpadButton::is_target () {
    return toggl_condition;
}

std::string CInpadButton::get_name () {
    return name;
}
