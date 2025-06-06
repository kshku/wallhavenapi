#include "whapi/whraw.h"

#include <assert.h>

#include "whcommon.h"

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

    whStr copy = whstr_create();
    CHECKB_RETURN(whstr_setn(&copy, res->str, res->len), false,
                  whstr_destroy(&copy));

    do {
        CHECKB_RETURN(whstr_setn(res, copy.str, copy.len), false,
                      whstr_destroy(&copy));
        CHECKB_RETURN(perform_call(), false, whstr_destroy(&copy));
    } while (whapi.retry);

    whstr_destroy(&copy);

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

bool whapi_get_tag_info_raw(const char *id, whStr *res) {
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

bool whapi_get_wallpapers_from_collection_raw(const char *user_name,
                                              const char *id,
                                              unsigned int purity, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_wallpaper_from_collection_url(user_name, id, purity),
                  false);

    return setup_and_fetch_data(res);
}
