#ifndef COBJECTSCONTAINER_H
#define COBJECTSCONTAINER_H

#include "cobject.h"
#include "../../patterns/CSmartList.h"

#include <QWidget>

class CObjectsContainer final : public CSmartList<CObject>
{
    Q_OBJECT
public:
    CObjectsContainer();
private slots:
    void deletionSignals ();
    void RMB(const QPoint& pos, CObject* target) override;
};

#endif // COBJECTSCONTAINER_H
