#pragma once

#include <string.h>

#include "whapi/whapi.h"

/**
 * @brief To manage strings.
 */
typedef struct whStr {
        size_t size;
        size_t len;
        char *str;
} whStr;

/**
 * @brief Set the string (Can call multiple times).
 *
 * This function will allocate memory as requried.
 *
 * @param whstr Pointer to whStr
 * @param str The string to set (will be copied)
 *
 * @return Returns true if string is set, otherwise false.
 */
WH_API bool whstr_set(whStr *whstr, const char *str);

/**
 * @brief Free the string.
 *
 * @param whstr Pointer to whStr.
 */
WH_API void whstr_free(whStr *whstr);
