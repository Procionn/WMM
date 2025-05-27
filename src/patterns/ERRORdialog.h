#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include "CFastDialog.h"
#include <QLabel>

class ERRORdialog : public CFastDialog
{
    Q_OBJECT
public:
    ERRORdialog(const std::string& label);
};





class FatalError : public CFastDialog
{
    Q_OBJECT
    bool fatality;
public:
    FatalError(const std::string& error, bool fatality = false);
    ~FatalError();
};

#endif // ERRORDIALOG_H
