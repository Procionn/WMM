#ifndef CINPADLIST_H
#define CINPADLIST_H

#include "../../patterns/CSmartList.h"
#include "CInpadButton.h"

class CInpadList final : public CSmartList<CInpadButton>
{
    Q_OBJECT
public:
    CInpadList();
private slots:
    void controller (QMouseEvent* event, CSmartObject* target) override;
};

#endif // CINPADLIST_H
