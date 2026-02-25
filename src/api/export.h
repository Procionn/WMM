#ifndef EXPORT_H
#define EXPORT_H

#ifdef EXPORT_SYMBOLS
    #ifdef _WIN32
        #ifdef WMMCORE
            #define WMMCORE_EXPORT __declspec(dllexport)
        #else
            #define WMMCORE_EXPORT __declspec(dllimport)
        #endif
    #else
        #define WMMCORE_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #define WMMCORE_EXPORT
#endif

#endif // EXPORT_H
