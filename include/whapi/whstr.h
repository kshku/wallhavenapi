#pragma once

#include "whapi/whdef.h"

/**
 * @brief To manage strings.
 */
typedef struct whStr {
        char *str; /**< Pointer to string */
        size_t size; /**< Size allocated */
        size_t len; /**< Length of the string */
} whStr;

/**
 * @brief Create whStr (Just initialize to 0 or NULL).
 *
 * @return Returns initialized whStr
 */
WH_API whStr whstr_create(void);

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
 * @brief Set the string of given length (Can call multiple times).
 *
 * This function will allocate memory as requried.
 *
 * @param whstr Pointer to whStr
 * @param str The string to set (will be copied)
 * @param len Length of string
 *
 * @return Returns true if string is set, otherwise false.
 */
WH_API bool whstr_setn(whStr *whstr, const char *str, size_t len);

/**
 * @brief Set the string after formating (Can call multiple times).
 *
 * This function will allocate memory as requried.
 *
 * @param whstr Pointer to whStr
 * @param fmt The format
 * @param ... Arguments
 *
 * @return Returns true if string is set, otherwise false.
 */
WH_API bool whstr_setf(whStr *whstr, const char *fmt, ...);

/**
 * @brief Free the string.
 *
 * @param whstr Pointer to whStr.
 */
WH_API void whstr_destroy(whStr *whstr);

/**
 * @brief Append given string.
 *
 * Allocation is handled.
 *
 * @param whstr Pointer to whStr
 * @param str String to append
 *
 * @return Returns false on failure.
 */
WH_API bool whstr_append(whStr *whstr, const char *str);

/**
 * @brief Append string of given length.
 *
 * Allocation is handled.
 *
 * @param whstr Pointer to whStr
 * @param str String to append
 * @param len lenght of string to append
 *
 * @return Returns false on failure.
 */
WH_API bool whstr_appendn(whStr *whstr, const char *str, size_t len);

/**
 * @brief Append string after formating.
 *
 * Allocation is handled.
 *
 * @param whstr Pointer to whStr
 * @param fmt The format
 * @param ... Arguments
 *
 * @return Returns false on failure.
 */
WH_API bool whstr_appendf(whStr *whstr, const char *fmt, ...);

/**
 * @brief Clear the string.
 *
 * Doesn't deallocate.
 *
 * @param whstr Pointer to whStr
 */
WH_API void whstr_clear(whStr *whstr);
