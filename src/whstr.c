#include "whapi/whstr.h"

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
        CHECKP_RETURN(temp, false);
        whstr->size = len + 1;
        whstr->str = temp;
    }

    // Copy the string
    whstr->len = len;
    strncpy(whstr->str, str, len);
    whstr->str[whstr->len] = 0;

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
        CHECKP_RETURN(temp, false);
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

void whstr_clear(whStr *whstr) {
    whstr->len = 0;
    whstr->str[0] = 0;
}
