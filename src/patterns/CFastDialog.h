#ifndef CFASTDIALOG_H
#define CFASTDIALOG_H

#include "../lang.h"
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CFastDialog : public QDialog
{
    Q_OBJECT
public:
    QWidget* list;
    QPushButton* apply;

    CFastDialog();
signals:
    void applyClicked();
    void canselClicked();
};

#endif // CFASTDIALOG_H
