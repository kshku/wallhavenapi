#include "whapi/whapi.h"

#include <assert.h>
#include <curl/curl.h>
#include <stdlib.h>

#include "whstr.h"

typedef struct whAPI {
        bool initialized;
        bool initialized_curl;

        CURL *curl;
        CURLU *url;

        whStr apikey;
} whAPI;

static whAPI whapi = {0};

bool whapi_initialize(bool init_curl) {
    if (whapi.initialized) return false;

    if (init_curl) curl_global_init(CURL_GLOBAL_ALL);
    whapi.initialized_curl = init_curl;

    whapi.curl = curl_easy_init();
    if (!whapi.curl) return false;

    whapi.url = curl_url();
    if (!whapi.url) return false;

    whapi.initialized = true;

    return true;
}

void whapi_shutdown(void) {
    assert(whapi.initialized);

    whstr_free(&whapi.apikey);

    if (whapi.url) curl_url_cleanup(whapi.url);

    if (whapi.curl) curl_easy_cleanup(whapi.curl);

    if (whapi.initialized_curl) curl_global_cleanup();
}

bool whapi_set_apikey(const char *apikey) {
    assert(whapi.initialized);

    // if NULL, unset
    if (!apikey) {
        whstr_free(&whapi.apikey);
        return true;
    }

    return whstr_set(&whapi.apikey, apikey);
}

void print_apikey(void) {
    assert(whapi.initialized);

    if (whapi.apikey.len) printf("%s\n", whapi.apikey.str);
    else printf("API key is not set\n");
}
