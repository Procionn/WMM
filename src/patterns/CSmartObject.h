#ifndef CSMARTOBJECT_H
#define CSMARTOBJECT_H

#include <QFrame>

class CSmartObject : public QFrame
{
    Q_OBJECT
public:
    bool toggl_condition = false;
public:
    CSmartObject(QWidget *parent = nullptr);
    void ChangeOfState();

// ////////////////////////////////////////////// You can override these 3 methods if you need to.
    virtual void context(const QPoint& pos);   // The "context" method, in my case, was used to open
    virtual void turnOff();                    // the context menu when pressing the RMB
    virtual void turnOn();                     //
// //////////////////////////////////////////////

protected:
    void mousePressEvent (QMouseEvent* event);
signals:
    void clicked(QMouseEvent* event, CSmartObject* target);
    void rightClick(const QPoint& pos, CSmartObject* target);
};

#endif // CSMARTOBJECT_H
