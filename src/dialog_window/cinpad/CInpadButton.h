#ifndef CINPADBUTTON_H
#define CINPADBUTTON_H

#include "../../patterns/CSmartObject.h"

#include <QLabel>
#include <QVBoxLayout>

class CInpadButton final : public CSmartObject
{
    std::string name;
    QLabel* label;
public:
    bool type;
    CInpadButton(const std::string& setName, const bool& type);
    void turnOff() override;
    void turnOn() override;
    bool is_target();
    std::string get_name();
protected:
    std::string untoggledColor = " background-color: #1e1e1e;";
    std::string toggledColor = " background-color: #444b52;";
};

#endif // CINPADBUTTON_H
