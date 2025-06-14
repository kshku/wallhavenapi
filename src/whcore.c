#include "whapi/whcore.h"

#include <assert.h>

#include "whcommon.h"

static bool default_response_code_handler(unsigned int response_code);

bool whapi_initialize(bool init_curl) {
    if (whapi.initialized) {
        whapi.error_code = WALLHAVEN_MULTIPLE_INITIALIZE,
        whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
        return false;
    }

    if (init_curl)
        CHECK_RETURN(whapi.error_code = curl_global_init(CURL_GLOBAL_ALL),
                     false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);
    whapi.initialized_curl = init_curl;

    CHECKP_RETURN(whapi.curl = curl_easy_init(), false,
                  whapi.error_code = WALLHAVEN_CURL_INIT_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKP_RETURN(whapi.url = curl_url(), false,
                  whapi.error_code = WALLHAVEN_CURL_INIT_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECK_RETURN(whapi.error_code = curl_url_set(
                     whapi.url, CURLUPART_URL, WALLHAVEN_URL, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whapi.apikey = whstr_create();

    whapi.response_code_handler = default_response_code_handler;

    whapi.initialized = true;

    whapi.error_code = 0;
    whapi.error_code_type = ERROR_CODE_TYPE_NONE;
    whapi.retry = false;

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

void whapi_get_last_error_code(unsigned int *code, ErrorCodeType *type) {
    *code = whapi.error_code;
    *type = whapi.error_code_type;
}

void whapi_set_response_code_handler(response_code_handler handler) {
    whapi.response_code_handler = handler;
}

static bool default_response_code_handler(unsigned int response_code) {
#ifdef WH_DEBUG
    printf("Response code: %u\n", response_code);
#endif
    (void)response_code;
    return false;
}
