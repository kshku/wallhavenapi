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

#ifdef WH_DEBUG
    char *path, buffer[4096];
    path = buffer;
    curl_url_get(whapi.url, CURLUPART_URL, &path, CURLU_URLDECODE);
    printf("URL: %s\n", path);
    // printf("CURLcode: %d\n", c);
    // printf("Response code: %ld\n", response_code);
#endif

    curl_easy_perform(whapi.curl);

    return true;
}
