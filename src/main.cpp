#include "core.h"
#include "lang.h"
#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    try {
        CConfigs config;
        config.config_test();
        Lang lang;
        lang.set_lang();

        QApplication app(argc, argv);
        app.setApplicationName("Wirus Mod Manager");
        QApplication::setStyle("Fusion");
        Window w;
        w.resize(1200, 800);
        w.show();

        return app.exec();
    } catch(const char* error) {
        std::cerr << error << std::endl;
    }
    return 0;
}
