#ifndef CTOGGLEDBUTTON_H
#define CTOGGLEDBUTTON_H

#include <QPushButton>
#include <string>

class CToggledButton : public QPushButton
{
    Q_OBJECT
public:
    std::string name = "";
    bool selected;

    CToggledButton(std::string setName = "", bool type = false);
    CToggledButton(QWidget* parent = nullptr, std::string setName = "", bool type = false);
    // CToggledButton(QVBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
    // CToggledButton(QHBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
    void SetLeftAlignment(bool type);
    void setTheme(std::string type);
    void setColor(std::string toggled, std::string untoggled);
    void isTarget(bool type);
private:
    std::string TBC1 = "=>";
    std::string TBC2 = "<=";
    bool leftAlignment = false;
    std::string leftAlign = "text-align: left; padding-left: 10px;";
    std::string untoggledColor = " background-color: #31363b;";
    std::string toggledColor = " background-color: #444b52;";
private slots:
    virtual void ChangeOfState();
signals:
    void toggled(CToggledButton* toggledButton, std::string name);
    void untoggled(CToggledButton* toggledButton, std::string name);
private:
    friend class CSwitchButton;
    friend class CLinkTumbler;
    friend class CInpadsButton;
};

#endif // CTOGGLEDBUTTON_H
