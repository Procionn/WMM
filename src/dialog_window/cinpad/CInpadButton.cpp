#include "CInpadButton.h"

CInpadButton::CInpadButton(const std::string& setName, const bool& type, bool& counter) {
    this->type = type;
    name = setName;
    QVBoxLayout* list = new QVBoxLayout(this);
    label = new QLabel(QString::fromStdString(setName));
    list->addWidget(label);
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
    if (counter) {
        setStyleSheet(QString::fromStdString(untoggledColor1));
        counter = false;
    }
    else {
        setStyleSheet(QString::fromStdString(untoggledColor2));
        counter = true;
    }
    count_type = counter;
}


void CInpadButton::turnOff () {
    if (toggl_condition) {
        toggl_condition = false;
        if (count_type)
            setStyleSheet(QString::fromStdString(untoggledColor2));
        else
            setStyleSheet(QString::fromStdString(untoggledColor1));
    }
}

void CInpadButton::turnOn () {
    if (!toggl_condition) {
        toggl_condition = true;
        if (count_type)
            setStyleSheet(QString::fromStdString(toggledColor2));
        else
            setStyleSheet(QString::fromStdString(toggledColor1));
    }
}

bool CInpadButton::is_target () {
    return toggl_condition;
}

std::string CInpadButton::get_name () {
    return name;
}
