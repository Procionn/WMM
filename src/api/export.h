#ifndef EXPORT_H
#define EXPORT_H

#ifdef WMMAPI
    #ifdef _WIN32
        #ifdef BUILDING_WMML_LIB
            #define WMMAPI_EXPORT __declspec(dllexport)
        #else
            #define WMMAPI_EXPORT __declspec(dllimport)
        #endif
    #else
        #define WMMAPI_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #define WMMAPI_EXPORT
#endif

#endif // EXPORT_H
