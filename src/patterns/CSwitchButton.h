#ifndef CSWITCHBUTTON_H
#define CSWITCHBUTTON_H

#include "CToggledButton.h"

class CSwitchButton : public CToggledButton
{
    Q_OBJECT
public:
    CSwitchButton(QWidget* parent = nullptr);
private slots:
    void ChangeOfState();
signals:
    void toggled(CSwitchButton* toggledButton);
    void untoggled(CSwitchButton* toggledButton);
};

#endif // CSWITCHBUTTON_H
