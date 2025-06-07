#include "whapi/whparsed.h"

#include <stdlib.h>

#include "cJSON.h"
#include "parse_funcs.h"
#include "whapi/whraw.h"
#include "whcommon.h"

bool whapi_get_wallpaper_info(const char *id, Wallpaper *wallpaper) {
    *wallpaper = (Wallpaper){0};

    whStr res = whstr_create();

    CHECKB_RETURN(whapi_get_wallpaper_info_raw(id, &res), false,
                  whstr_destroy(&res));

    wallpaper->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(wallpaper->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_wallpaper_info(wallpaper->json, wallpaper, false),
                  false, whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

bool whapi_search(SearchParameters params, SearchResult *search_result) {
    *search_result = (SearchResult){0};

    whStr res = whstr_create();

    CHECKB_RETURN(whapi_search_raw(params, &res), false, whstr_destroy(&res));

    search_result->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(search_result->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_search_result(search_result->json, search_result,
                                      params.q.id != 0, false),
                  false, whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

bool whapi_get_tag_info(unsigned int id, Tag *tag) {
    *tag = (Tag){0};

    whStr res = whstr_create();

    CHECKB_RETURN(whapi_get_tag_info_raw(id, &res), false, whstr_destroy(&res));

    tag->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(tag->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_tag(tag->json, tag, false), false, whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

bool whapi_get_settings(Settings *settings) {
    *settings = (Settings){0};

    whStr res = whstr_create();

    CHECKB_RETURN(whapi_get_settings_raw(&res), false, whstr_destroy(&res));

    settings->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(settings->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_settings(settings->json, settings), false,
                  whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

bool whapi_get_collections(const char *user_name, Collections *collections) {
    *collections = (Collections){0};

    whStr res = whstr_create();

    CHECKB_RETURN(whapi_get_collections_raw(user_name, &res), false,
                  whstr_destroy(&res));

    collections->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(collections->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_collections(collections->json, collections), false,
                  whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

bool whapi_get_wallpapers_from_collection(const char *user_name,
                                          unsigned int id, unsigned int purity,
                                          SearchResult *search_result) {
    *search_result = (SearchResult){0};

    whStr res = whstr_create();

    CHECKB_RETURN(
        whapi_get_wallpapers_from_collection_raw(user_name, id, purity, &res),
        false, whstr_destroy(&res));

    search_result->json = cJSON_ParseWithLength(res.str, res.len);
    CHECKP_RETURN(search_result->json, false, whstr_destroy(&res);
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(parse_search_result(search_result->json, search_result,
                                      false /* doesn't matter */, true),
                  false, whstr_destroy(&res));

    whstr_destroy(&res);

    return true;
}

void whapi_destroy_tag(Tag *tag) {
    if (tag->json) cJSON_Delete(tag->json);

    *tag = (Tag){0};
}

void whapi_destroy_settings(Settings *settings) {
    free(settings->resolutions);

    free(settings->aspect_ratios);

    free(settings->tag_blacklist);

    free(settings->user_blacklist);

    if (settings->json) cJSON_Delete(settings->json);

    *settings = (Settings){0};
}

void whapi_destroy_wallpaper(Wallpaper *wallpaper) {
    free(wallpaper->colors);
    free(wallpaper->tags);

    if (wallpaper->json) cJSON_Delete(wallpaper->json);

    *wallpaper = (Wallpaper){0};
}

void whapi_destroy_search_result(SearchResult *search_result) {
    for (size_t i = 0; i < search_result->wallpaper_count; ++i)
        whapi_destroy_wallpaper(&search_result->wallpapers[i]);

    free(search_result->wallpapers);

    if (search_result->json) cJSON_Delete(search_result->json);

    *search_result = (SearchResult){0};
}

void whapi_destroy_collections(Collections *collections) {
    free(collections->collections);

    if (collections->json) cJSON_Delete(collections->json);

    *collections = (Collections){0};
}
