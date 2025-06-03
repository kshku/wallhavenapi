#include "whstr.h"

#include <stdlib.h>

bool whstr_set(whStr *whstr, const char *str) {
    size_t len = strlen(str);

    // Resize if required
    if (whstr->size < len) {
        char *temp = realloc(whstr->str, sizeof(char) * (len + 1));
        if (!temp) return false;
        whstr->size = len + 1;
        whstr->str = temp;
    }

    // Copy the key
    whstr->len = len;
    strncpy(whstr->str, str, len);
    whstr->str[len] = 0;

    return true;
}

void whstr_free(whStr *whstr) {
    if (whstr->size) free(whstr->str);
    whstr->len = whstr->size = 0;
    whstr->str = NULL;
}
