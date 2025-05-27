#ifndef CFASTDIALOG_H
#define CFASTDIALOG_H

#include "../core.h"
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CFastDialog : public QDialog
{
    Q_OBJECT
protected:
    QHBoxLayout* DialogButtonBox;
    bool autodeleting = true;

public:
    QWidget* list;
    QPushButton* apply;

    CFastDialog();
signals:
    void applyClicked();
    void canselClicked();
};

#endif // CFASTDIALOG_H
