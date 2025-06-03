#pragma once

#include <stdbool.h>

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

/**
 * @brief Initialize whapi.
 *
 * Initialize and optionally initialize curl.
 *
 * @note If application calls curl_global_init, then init_curl should be false.
 *
 * @param init_curl initialize curl
 *
 * @return Returns true on success, otherwise false.
 */
WH_API bool whapi_initialize(bool init_curl);

/**
 * @brief Shutdown whapi.
 *
 * @note If was whapi was initialized with init_curl true, then this will call
 * curl_global_cleanup.
 */
WH_API void whapi_shutdown(void);

/**
 * @brief Set the wallhaven API key.
 *
 * @note To unset the API key, just pass NULL.
 *
 * @param apikey The API key
 *
 * @return Returns true on succes, otherwise false.
 */
WH_API bool whapi_set_apikey(const char *apikey);

WH_API void print_apikey(void);
