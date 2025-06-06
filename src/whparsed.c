#include "whapi/whparsed.h"

#include <stdlib.h>

#include "whapi/whraw.h"
#include "whcommon.h"

bool whapi_get_wallpaper_info(const char *id, Wallpaper *wallpaper) {
    *wallpaper = (Wallpaper){.res = whstr_create()};

    CHECKB_RETURN(whapi_get_wallpaper_info_raw(id, &wallpaper->res), false,
                  whstr_destroy(&wallpaper->res));

    return true;
}

bool whapi_search(SearchParameters params, SearchResult *search_result) {
    // *search_result = (SearchResult){.res = whstr_create()};
    search_result->res = whstr_create();

    CHECKB_RETURN(whapi_search_raw(params, &search_result->res), false,
                  whstr_destroy(&search_result->res));

    return true;
}

bool whapi_get_tag_info(unsigned int id, Tag *tag) {
    *tag = (Tag){.res = whstr_create()};

    CHECKB_RETURN(whapi_get_tag_info_raw(id, &tag->res), false,
                  whstr_destroy(&tag->res));

    return true;
}

bool whapi_get_settings(Settings *settings) {
    *settings = (Settings){.res = whstr_create()};

    CHECKB_RETURN(whapi_get_settings_raw(&settings->res), false,
                  whstr_destroy(&settings->res));

    return true;
}

bool whapi_get_collections(const char *user_name, Collections *collections) {
    *collections = (Collections){.res = whstr_create()};

    CHECKB_RETURN(whapi_get_collections_raw(user_name, &collections->res),
                  false, whstr_destroy(&collections->res));

    return true;
}

bool whapi_get_wallpapers_from_collection(const char *user_name,
                                          unsigned int id, unsigned int purity,
                                          SearchResult *search_result) {
    search_result->res = whstr_create();

    CHECKB_RETURN(whapi_get_wallpapers_from_collection_raw(
                      user_name, id, purity, &search_result->res),
                  false, whstr_destroy(&search_result->res));

    return true;
}

void whapi_destroy_tag(Tag *tag) {
    whstr_destroy(&tag->res);
}

void whapi_destroy_settings(Settings *settings) {
    free(settings->resolutions);

    free(settings->aspect_ratios);

    free(settings->tag_blacklists);

    free(settings->user_blacklists);

    whstr_destroy(&settings->res);
}

void whapi_destroy_wallpaper(Wallpaper *wallpaper) {
    free(wallpaper->colors);
    free(wallpaper->tags);

    whstr_destroy(&wallpaper->res);
}

void whapi_destroy_search_result(SearchResult *search_result) {
    for (size_t i = 0; i < search_result->wallpaper_count; ++i)
        whapi_destroy_wallpaper(&search_result->wallpapers[i]);

    free(search_result->wallpapers);

    whstr_destroy(&search_result->res);
}

void whapi_destroy_collections(Collections *collections) {
    free(collections->collections);

    whstr_destroy(&collections->res);
}
