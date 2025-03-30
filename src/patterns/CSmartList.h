#ifndef CSMARTLIST_H
#define CSMARTLIST_H

#include "templatelist.h"
#include "CSmartObject.h"

#include <vector>
#include <QMouseEvent>

template <class T>
class CSmartList : public TemplateList
{
protected:
    std::vector<T*> childList;
    T* lastTarget;
public:
    CSmartList() {
        static_assert(std::is_base_of<CSmartObject, T>::value, "The parameter must be inherited from CSmartObject.");
    }


    void add (T* target) {
        this->list->addWidget(target);
        childList.emplace_back(target);
        connecting(target);
    }


    void clear () {
        for (auto target : childList)
            delete target;
        childList.clear();
    }


private:

    void multipleСhoice (T* target) {
        if (target == lastTarget) return;
        if (lastTarget != nullptr) {
            int max;
            int min;
            bool tmp = true;
            for (int i = 0; i < childList.size(); ++i) {
                if (childList[i] == target ||
                    childList[i] == lastTarget) {
                    if (tmp) {
                        tmp = false;
                        min = i;
                    }
                    else max = i;
                }
            }
            for (int i = min; i <= max; ++i)
                childList[i]->turnOn();
        }
        else {
            target->ChangeOfState();
            lastTarget = target;
        }
    }



private slots:

    void reset () {
        for (auto target : childList) {
            target->turnOff();
            lastTarget = nullptr;
        }
    }


    void controller (QMouseEvent* event, CSmartObject* target) override {
        if (event->button() == Qt::LeftButton) {
            if (event->modifiers() == Qt::ControlModifier)
                LBM_CTRL(static_cast<T*>(target));
            else if (event->modifiers() == Qt::ShiftModifier)
                LBM_SHIFT(static_cast<T*>(target));
            else reset();
        }
#if 0
        for (T* t : childList)
            std::cout << std::left << std::setw(30) << t->name  << " " << t->toggl_condition << std::endl;
        for (int i = 0; i != 3; ++i)
            std::cout << std::endl;
#endif
    }


    void rightClickController (const QPoint& pos, CSmartObject* target) override {
        if (lastTarget == nullptr)
            target->context(pos);
        else
            RMB(pos, static_cast<T*>(target));
    }



protected slots:

    virtual void RMB (const QPoint& pos, T* target) {}


    virtual void LBM_SHIFT (T* target) {
        multipleСhoice(target);
    }


    virtual void LBM_CTRL (T* target) {
        target->ChangeOfState();
        lastTarget = target;
    }
};

#endif // CSMARTLIST_H
