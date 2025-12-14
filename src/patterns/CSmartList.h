/*
 *  Copyright (C) 2025 Procion ByProcion@gmail.com
 *
 *  This file is part of Wirus Mod Manager.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 *
 */
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
    T* lastTarget = nullptr;
public:
    CSmartList(QWidget* parent = nullptr) : TemplateList(parent) {
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
                if(childList[i]->isVisible())
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
        }
        lastTarget = nullptr;
        reseted();
    }


    void controller (QMouseEvent* event, CSmartObject* target) override {
        if (event->button() == Qt::LeftButton) {
            if (event->modifiers() == Qt::ControlModifier)
                LBM_CTRL(static_cast<T*>(target));
            else if (event->modifiers() == Qt::ShiftModifier)
                LBM_SHIFT(static_cast<T*>(target));
            else reset();
            updated();
        }
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
