#include "whapi/whcore.h"

#include <assert.h>

#include "whcommon.h"

bool whapi_initialize(bool init_curl) {
    if (whapi.initialized) return false;

    whapi.error_code = 0;
    whapi.error_code_type = ERROR_CODE_TYPE_NONE;

    if (init_curl)
        CHECK_RETURN(whapi.error_code = curl_global_init(CURL_GLOBAL_ALL),
                     false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);
    whapi.initialized_curl = init_curl;

    CHECKP_RETURN(whapi.curl = curl_easy_init(), false);

    CHECKP_RETURN(whapi.url = curl_url(), false);

    CHECK_RETURN(whapi.error_code = curl_url_set(
                     whapi.url, CURLUPART_URL, WALLHAVEN_URL, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whapi.apikey = whstr_create();

    whapi.initialized = true;

    return true;
}

void whapi_shutdown(void) {
    assert(whapi.initialized);

    whstr_destroy(&whapi.apikey);

    if (whapi.url) curl_url_cleanup(whapi.url);

    if (whapi.curl) curl_easy_cleanup(whapi.curl);

    if (whapi.initialized_curl) curl_global_cleanup();

    whapi.initialized = false;
}

bool whapi_set_apikey(const char *apikey) {
    assert(whapi.initialized);

    // if NULL, unset
    CHECKP_RETURN(apikey, true, whstr_destroy(&whapi.apikey));

    return whstr_set(&whapi.apikey, apikey);
}

void print_apikey(void) {
    assert(whapi.initialized);

    if (whapi.apikey.str) printf("%s\n", whapi.apikey.str);
    else printf("API key is not set\n");
}

void whapi_get_last_error_code(unsigned int *code, ErrorCodeType *type) {
    *code = whapi.error_code;
    *type = whapi.error_code_type;
}
