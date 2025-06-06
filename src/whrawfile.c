#include "whapi/whrawfile.h"

#include <assert.h>

#include "whcommon.h"

static size_t write_to_file(void *buffer, size_t size, size_t nmemb,
                            void *userp) {
    FILE *fp = (FILE *)userp;
    return fwrite(buffer, size, nmemb, fp);
}

static bool setup_and_fetch_data(const char *filename) {
    CHECK_RETURN(whapi.error_code =
                     curl_easy_setopt(whapi.curl, CURLOPT_CURLU, whapi.url),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    CHECK_RETURN(whapi.error_code = curl_easy_setopt(
                     whapi.curl, CURLOPT_WRITEFUNCTION, write_to_file),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    do {
        FILE *file = fopen(filename, "w");
        CHECKP_RETURN(file, false,
                      whapi.error_code = WALLHAVEN_OPEN_FILE_FAILED,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

        CHECK_RETURN(whapi.error_code = curl_easy_setopt(
                         whapi.curl, CURLOPT_WRITEDATA, (void *)file),
                     false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

        CHECKB_RETURN(perform_call(), false);

        fclose(file);
    } while (whapi.retry);

    return true;
}

bool whapi_get_wallpaper_info_raw_to_file(const char *id, const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_wallpaper_info_url(id), false);

    return setup_and_fetch_data(file);
}

bool whapi_search_raw_to_file(SearchParameters params, const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_search_url(&params), false);

    return setup_and_fetch_data(file);
}

bool whapi_get_tag_info_raw_to_file(unsigned int id, const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_tag_info_url(id), false);

    return setup_and_fetch_data(file);
}

bool whapi_get_settings_raw_to_file(const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_settings_url(), false);

    return setup_and_fetch_data(file);
}

bool whapi_get_collections_raw_to_file(const char *user_name,
                                       const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_collections_url(user_name), false);

    return setup_and_fetch_data(file);
}

bool whapi_get_wallpapers_from_collection_raw_to_file(const char *user_name,
                                                      unsigned int id,
                                                      unsigned int purity,
                                                      const char *file) {
    assert(whapi.initialized);

    CHECKB_RETURN(setup_wallpaper_from_collection_url(user_name, id, purity),
                  false);

    return setup_and_fetch_data(file);
}
