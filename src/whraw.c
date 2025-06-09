#include "whapi/whraw.h"

#include <assert.h>

#include "whcommon.h"

static size_t write_to_whstr(void *buffer, size_t size, size_t nmemb,
                             void *userp) {
    size_t realsize = size * nmemb;
    whStr *r = (whStr *)userp;

    CHECKB_RETURN(whstr_appendn(r, buffer, realsize), 0);

    return realsize;
}

static bool setup_and_fetch_data(whStr *res) {
    CHECK_RETURN(whapi.error_code =
                     curl_easy_setopt(whapi.curl, CURLOPT_CURLU, whapi.url),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    CHECK_RETURN(whapi.error_code = curl_easy_setopt(
                     whapi.curl, CURLOPT_WRITEFUNCTION, write_to_whstr),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    CHECK_RETURN(whapi.error_code = curl_easy_setopt(
                     whapi.curl, CURLOPT_WRITEDATA, (void *)res),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    do {
        whstr_clear(res);
        CHECKB_RETURN(perform_call(), false);
    } while (whapi.retry);

    return true;
}

bool whapi_get_wallpaper_info_raw(const char *id, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_wallpaper_info_url(id), false);

    return setup_and_fetch_data(res);
}

bool whapi_search_raw(SearchParameters params, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_search_url(&params), false);

    return setup_and_fetch_data(res);
}

bool whapi_get_tag_info_raw(size_t id, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_tag_info_url(id), false);

    return setup_and_fetch_data(res);
}

bool whapi_get_settings_raw(whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_settings_url(), false);

    return setup_and_fetch_data(res);
}

bool whapi_get_collections_raw(const char *user_name, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_collections_url(user_name), false);

    return setup_and_fetch_data(res);
}

bool whapi_get_wallpapers_from_collection_raw(const char *user_name, size_t id,
                                              unsigned int purity, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_wallpaper_from_collection_url(user_name, id, purity),
                  false);

    return setup_and_fetch_data(res);
}
