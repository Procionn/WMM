#ifndef EXPORT_H
#define EXPORT_H

#ifdef _WIN32
    #ifdef WMMAPI
        #define WMMAPI_EXPORT __declspec(dllexport)
    #else
        #define WMMAPI_EXPORT __declspec(dllimport)
    #endif
#else
    #define WMMAPI_EXPORT __attribute__((visibility("default")))
#endif

#endif // EXPORT_H
