#include "CSwitchButton.h"

CSwitchButton::CSwitchButton(QWidget* parent) : CToggledButton(parent){setText("OFF");}

void CSwitchButton::ChangeOfState() {
    if (selected) {
        emit untoggled(this);
        this->setText("OFF");
        selected = false;
        setStyleSheet(QString::fromStdString(untoggledColor));
    }
    else {
        emit toggled(this);
        this->setText("ON");
        selected = true;
        setStyleSheet(QString::fromStdString(toggledColor));
    }
}
