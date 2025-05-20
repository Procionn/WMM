#ifndef CINPAD_H
#define CINPAD_H

#include "../../patterns/CFastDialog.h"
#include "../../CONSTANTS.h"
#include "../../methods.h"
#include "../../patterns/CScrollWindow.h"
#include "../../window/CObjectsButton.h"
#include "CInpadButton.h"
#include "CInpadList.h"

#include <vector>
#include <iostream>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QLabel>



struct Cbox
{
    std::string path = "";
};



class CInpad : public CFastDialog
{
    Q_OBJECT
public:
    bool targetType = false;
    bool* callType;
    CInpadList* newObjectList;
    CObjectsButton** target;

    CInpad(bool& type);
    void reset();
    void application(std::string& targetName, bool targetType);
private:
    std::vector<CInpadButton*> vlist;
    bool vector = false;
    QMenu* menu;
    QAction* presets;

    void render();
    void reader();
    void fsScaner(const std::filesystem::path& directory, const bool& type,
                  const int& arraySize, std::vector<Cbox>& existsElements, bool& count_type);
    void distributor();
    bool nameTest(std::vector<Cbox>& existsElements, int indicator, std::string str);
};

#endif // CINPAD_H
