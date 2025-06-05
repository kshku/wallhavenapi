#pragma once

#include "whapi/whdef.h"

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

/**
 * @brief Get the last error code from internally used libraries.
 *
 * @param code Pointer to store the error code
 * @param type Pointer to store the error code type
 */
WH_API void whapi_get_last_error_code(unsigned int *code, ErrorCodeType *type);

/**
 * @brief Set the response code handler.
 *
 * @param handler The response code handler
 */
WH_API void whapi_set_response_code_handler(response_code_handler handler);

// Just for testing purpose
WH_API void print_apikey(void);
