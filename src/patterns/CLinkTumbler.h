#ifndef CLINKTUMBLER_H
#define CLINKTUMBLER_H

#include "CToggledButton.h"

class CLinkTumbler : public CToggledButton
{
    Q_OBJECT
public:
    CLinkTumbler* link = nullptr;
    CLinkTumbler(std::string name = "", CLinkTumbler* linked = nullptr, QWidget* parent = nullptr);
protected slots:
    void ChangeOfState();
    void reset();
};


#endif // CLINKTUMBLER_H
