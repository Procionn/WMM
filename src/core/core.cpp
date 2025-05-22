#include "../core.h"

Core& Core::get () {
    static Core singleton;
    return singleton;
}
