#include "whcommon.h"

// Global state
whAPI whapi = {0};

size_t write_to_whstr(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    whStr *r = (whStr *)userp;

    CHECKB_RETURN(whstr_appendn(r, buffer, realsize), 0);

    return realsize;
}

size_t write_to_file(void *buffer, size_t size, size_t nmemb, void *userp) {
    FILE *fp = (FILE *)userp;
    return fwrite(buffer, size, nmemb, fp);
}

bool reset_url(void) {
    CHECK_RETURN(whapi.error_code = curl_url_set(
                     whapi.url, CURLUPART_URL, WALLHAVEN_URL, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    return true;
}

bool append_query(const char *key, const char *value) {
    whStr query = whstr_create();

    CHECKB_RETURN(whstr_set(&query, key), false, whstr_destroy(&query));
    CHECKB_RETURN(whstr_appendn(&query, "=", 1), false, whstr_destroy(&query));
    CHECKB_RETURN(whstr_append(&query, value), false, whstr_destroy(&query));

    CHECK_RETURN(
        whapi.error_code = curl_url_set(whapi.url, CURLUPART_QUERY, query.str,
                                        CURLU_APPENDQUERY | CURLU_URLENCODE),
        false, whstr_destroy(&query);
        whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whstr_destroy(&query);

    return true;
}

bool concat_and_set_path(const char *p, const char *q) {
    whStr path = whstr_create();

    CHECKB_RETURN(whstr_set(&path, p), false, whstr_destroy(&path));
    CHECKB_RETURN(whstr_append(&path, q), false, whstr_destroy(&path));

    CHECK_RETURN(whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                                 path.str, CURLU_URLENCODE),
                 false, whstr_destroy(&path);
                 whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whstr_destroy(&path);

    return true;
}
