#include "CInpadList.h"

CInpadList::CInpadList() {}


void CInpadList::controller (QMouseEvent* event, CSmartObject* target) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ControlModifier)
            LBM_CTRL(static_cast<CInpadButton*>(target));
        else if (event->modifiers() == Qt::ShiftModifier)
            LBM_SHIFT(static_cast<CInpadButton*>(target));
        else {
            target->ChangeOfState();
            lastTarget = static_cast<CInpadButton*>(target);
        }
    }
}
