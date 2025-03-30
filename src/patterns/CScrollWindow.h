#ifndef CSCROLLWINDOW_H
#define CSCROLLWINDOW_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

template <typename T, typename U>
void addScrollable (T* parent, U* child) {
    static_assert(std::is_base_of<QLayout, U>::value, "Child must be inherited from QLayout");
    QScrollArea* scroll = new QScrollArea;
    QWidget* scrolledWidget = new QWidget;
    scroll->setWidgetResizable(true);

    if constexpr (std::is_base_of<QWidget, T>::value) {
        QVBoxLayout* lay = new QVBoxLayout(parent);
        lay->addWidget(scroll);
    }
    else if constexpr (std::is_base_of<QLayout, T>::value) {
        parent->addWidget(scroll);
    }
    scroll->setWidget(scrolledWidget);
    scrolledWidget->setLayout(child);
}


#endif // CSCROLLWINDOW_H
