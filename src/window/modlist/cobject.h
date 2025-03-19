#ifndef COBJECT_H
#define COBJECT_H

#include <QLabel>
#include <string>

class CObject : public QFrame
{
    Q_OBJECT
public:
    std::string name;
public:
    CObject();
};

#endif // COBJECT_H
