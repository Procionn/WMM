#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFrame>
#include "../patterns/CLinkTumbler.h"

class SettingsBox : public QFrame
{
public:
    bool monolith = false;
    bool online = false;

    SettingsBox();
};



class collections : public QWidget
{
    Q_OBJECT
protected:
    CLinkTumbler* target = nullptr;
    QVBoxLayout* collections_list;
    QVBoxLayout* right_list;

    void update_collection_info(CToggledButton* target);
public:
    collections();

public slots:
    void update_list();

private slots:
    void exporting(SettingsBox* parameters);
    void importing();
};

#endif // COLLECTIONS_H
