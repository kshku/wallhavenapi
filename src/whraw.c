#include "whapi/whraw.h"

#include <assert.h>

#include "whcommon.h"

bool whapi_get_wallpaper_info_raw(const char *id, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(reset_url(), false);

    CHECKB_RETURN(concat_and_set_path(WALLPAPER_INFO_PATH, id), false);

    // TODO: If failed to use apikey, warn
    if (whapi.apikey.str) append_query("apikey", whapi.apikey.str);

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

bool whapi_search_raw(SearchParameters params, whStr *res) {
    assert(whapi.initialized);

    CHECKB_RETURN(reset_url(), false);

    CHECK_RETURN(whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                                 SEARCH_PATH, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    if (whapi.apikey.str)
        CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false);

    CHECKB_RETURN(format_and_append_search_parameters(&params), false);

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
