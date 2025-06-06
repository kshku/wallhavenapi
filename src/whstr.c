#include "whapi/whstr.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "whcommon.h"

WH_API whStr whstr_create(void) {
    return (whStr){.size = 0, .len = 0, .str = NULL};
}

bool whstr_set(whStr *whstr, const char *str) {
    return whstr_setn(whstr, str, strlen(str));
}

bool whstr_setn(whStr *whstr, const char *str, size_t len) {
    if (whstr->size == 0) whstr->str = NULL;

    // Resize if required
    if (whstr->size <= len) {
        char *temp = realloc(whstr->str, sizeof(char) * (len + 1));
        CHECKP_RETURN(temp, false, whapi.error_code = WALLHAVEN_ALLOCATION_FAIL,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);
        whstr->size = len + 1;
        whstr->str = temp;
    }

    // Copy the string
    whstr->len = len;
    strncpy(whstr->str, str, len);
    whstr->str[whstr->len] = 0;

    return true;
}

bool whstr_setf(whStr *whstr, const char *fmt, ...) {
    va_list args;
    size_t len;

    va_start(args, fmt);
    len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (whstr->size <= len) {
        char *temp = realloc(whstr->str, sizeof(char) * (len + 1));
        CHECKP_RETURN(temp, false, whapi.error_code = WALLHAVEN_ALLOCATION_FAIL,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);
        whstr->size = len + 1;
        whstr->str = temp;
    }

    va_start(args, fmt);
    whstr->len = vsnprintf(whstr->str, whstr->size, fmt, args);
    va_end(args);

    return true;
}

void whstr_destroy(whStr *whstr) {
    if (whstr->size) free(whstr->str);
    whstr->len = whstr->size = 0;
    whstr->str = NULL;
}

bool whstr_append(whStr *whstr, const char *str) {
    return whstr_appendn(whstr, str, strlen(str));
}

bool whstr_appendn(whStr *whstr, const char *str, size_t len) {
    if (whstr->size == 0) whstr->str = NULL;

    // Resize if required
    if (whstr->size <= whstr->len + len) {
        char *temp = realloc(whstr->str, sizeof(char) * (whstr->len + len + 1));
        CHECKP_RETURN(temp, false, whapi.error_code = WALLHAVEN_ALLOCATION_FAIL,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);
        whstr->size = len + whstr->len + 1;

        // If the string was null, then put null character in the beginning to
        // avoid unpredictable results
        if (!whstr->str) temp[0] = 0;

        whstr->str = temp;
    }

    // Copy the string
    whstr->len += len;
    strncat(whstr->str, str, len);
    whstr->str[whstr->len] = 0;

    return true;
}

bool whstr_appendf(whStr *whstr, const char *fmt, ...) {
    if (whstr->size == 0) whstr->str = NULL;

    va_list args;
    size_t len;

    va_start(args, fmt);
    len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (whstr->size <= whstr->len + len) {
        char *temp = realloc(whstr->str, sizeof(char) * (whstr->len + len + 1));
        CHECKP_RETURN(temp, false, whapi.error_code = WALLHAVEN_ALLOCATION_FAIL,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);
        whstr->size = len + whstr->len + 1;

        // If the string was null, then put null character in the beginning to
        // avoid unpredictable results
        if (!whstr->str) temp[0] = 0;

        whstr->str = temp;
    }

    va_start(args, fmt);
    whstr->len += vsnprintf((whstr->str + whstr->len), len + 1, fmt, args);
    va_end(args);

    return true;
}

void whstr_clear(whStr *whstr) {
    whstr->len = 0;
    whstr->str[0] = 0;
}
