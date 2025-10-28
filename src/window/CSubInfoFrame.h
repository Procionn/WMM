#ifndef CSUBINFOFRAME_H
#define CSUBINFOFRAME_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>

class CSubInfoFrame : public QHBoxLayout
{
    Q_OBJECT
public:
    QSplitter* spl1;
    QSplitter* spl2;
    CSubInfoFrame(QVBoxLayout* parent = nullptr);

signals:
};

#endif // CSUBINFOFRAME_H
