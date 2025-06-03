#pragma once

#if defined(__gnu_linux__) || defined(__linux__)
    #define WH_OS_LINUX
#elif defined(_WIN64)
    #define WH_OS_WINDOWS
#else
    #error "Don't know whether works!"
#endif

#ifdef WH_EXPORT
    #if defined(WH_OS_LINUX)
        #define WH_API __attribute__((visibility("default")))
    #elif defined(WH_OS_WINDOWS)
        #define WH_API __declspec(dllexport)
    #else
        #error "Should not reach here"
    #endif
#else
    #if defined(WH_OS_LINUX)
        #define WH_API
    #elif defined(WH_OS_WINDOWS)
        #define WH_API __declspec(dllimport)
    #else
        #error "Should not reach here"
    #endif
#endif

WH_API void print_hello(void);
