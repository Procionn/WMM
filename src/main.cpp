#include "window.h"
#include "core.h"
#include "lang.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    CConfigs config;
    config.config_test(); 
    Lang lang;
    lang.set_lang();

    
    QApplication a(argc, argv);
    Window w;
    w.resize(1200, 800);
    w.show();
    
    return a.exec();
}
