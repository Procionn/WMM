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
                stc::cerr(error);
                FatalError* error_dialog = new FatalError(error, true);
                error_dialog->exec();
            } catch (const std::exception& e) {
                stc::cerr(std::string("Error: ") + e.what());
                FatalError* error_dialog = new FatalError(e.what(), true);
                error_dialog->exec();
            } catch (...) {throw;}
        } catch (reset* action) {
            delete action;
            stc::cerr("Rebooting");
            continue;
        } catch (exit_signal* action) {
            delete action;
            stc::cerr("Exit...");
            return 10;
        }
/*
 * I know that exceptions are an extremely expensive language feature.
 * However, both reboots and program terminations are not called as often,
 * besides, during their execution, the speed of work is no longer very important.
 * In return, it allows me not to write a separate class inherited from QObject,
 * for the sake of reboots and forced shutdowns alone.
*/
    }
    return app.exec();
}
