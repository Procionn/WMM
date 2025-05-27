#include "core.h"
#include "window.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    while(true) {
        try {
            try {
                static Core& globalCore = Core::get();
                app.setApplicationName("Wirus Mod Manager");
                QApplication::setStyle("Fusion");
                Window w;
                w.resize(1200, 800);
                w.show();
                return app.exec();

            } catch(const char* error) {
                std::cerr << error << std::endl;
                FatalError* error_dialog = new FatalError(error, true);
                error_dialog->exec();
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                FatalError* error_dialog = new FatalError(e.what(), true);
                error_dialog->exec();
            } catch (...) {throw;}
        } catch (reset* action) {
            delete action;
            std::cerr << "Rebooting" << std::endl;
            continue;
        }

    }
    return app.exec();
}
