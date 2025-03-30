#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include "CFastDialog.h"
#include <QLabel>

class ERRORdialog : public CFastDialog
{
    Q_OBJECT
public:
    ERRORdialog(std::string label);
};

#endif // ERRORDIALOG_H
