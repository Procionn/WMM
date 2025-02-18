#ifndef PATTERNS_H
#define PATTERNS_H

#include <string>
#include <QFrame>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>

class CSwitchButton;
class CSplitter;

class CContentBox : public QFrame
{
    Q_OBJECT
public:
    int index;
    QHBoxLayout* Box;
    QLabel* name;
    QLabel* version;
    QLabel* type;
    CSwitchButton* switcher;
    CSplitter* spl1;
    CSplitter* spl2;
    CSplitter* spl3;
    
    CContentBox(QVBoxLayout* parent = nullptr);
signals:
    void ON(CContentBox* pointer);
    void OFF(CContentBox* pointer);
};





class CScrollWindow : public QScrollArea
{
    Q_OBJECT
public:
    CScrollWindow(QWidget* parent, QVBoxLayout* child);
    CScrollWindow(QWidget* parent, QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QVBoxLayout* child);
    CScrollWindow(QHBoxLayout *parent, QHBoxLayout* child);
    CScrollWindow(QVBoxLayout *parent, QHBoxLayout* child);
};





class CSplitter : public QSplitter
{
    Q_OBJECT
public:
    CSplitter(QHBoxLayout* parent);
public slots:
    void moveSplitter(int pos, int index);
};





class CToggledButton : public QPushButton
{
    Q_OBJECT
public:
    std::string name = "";
    bool selected;
    
    CToggledButton(std::string setName = "", bool type = false);
    CToggledButton(QWidget* parent = nullptr, std::string setName = "", bool type = false);
    CToggledButton(QVBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
    CToggledButton(QHBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
    void SetLeftAlignment(bool type);
    void setTheme(std::string type);
    void setColor(std::string toggled, std::string untoggled);
    void isTarget(bool type);
private:
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





class CLinkTumbler : public CToggledButton
{
    Q_OBJECT
public:
    CLinkTumbler* link = nullptr;
    CLinkTumbler(std::string name = "", CLinkTumbler* linked = nullptr, QWidget* parent = nullptr);
private slots:
    void ChangeOfState();
    void reset();
private:
    friend class CObjectsButton;
};





class CObjectsButton : public CLinkTumbler
{
    Q_OBJECT
public:
    bool type;
    CObjectsButton* link = nullptr;
    CObjectsButton(std::string name = "", CObjectsButton* linked = nullptr, QWidget* parent = nullptr);
};





class CInpadsButton : public CToggledButton
{
    Q_OBJECT
public:
    bool type = false;
    CInpadsButton(QVBoxLayout* parent = nullptr, std::string setName = "", bool type = false);
};

#endif // PATTERNS_H
