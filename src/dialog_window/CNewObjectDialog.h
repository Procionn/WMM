#ifndef CNEWOBJECTDIALOG_H
#define CNEWOBJECTDIALOG_H

#include "../patterns/CFastDialog.h"

#include <QLineEdit>
#include <QLabel>

class CNewObjectDialog : public CFastDialog
{
    Q_OBJECT
public:
    QLineEdit* nameTab;
    CNewObjectDialog();
};

#endif // CNEWOBJECTDIALOG_H
