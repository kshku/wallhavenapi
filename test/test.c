#include <stdio.h>
#include <whapi/whapi.h>

int main(void) {
    if (!whapi_initialize(true)) {
        whapi_shutdown();
        return -1;
    }

    whStr temp = whstr_create();

    SearchParameters search_params = {
        .q = {.tags = "mountain"},
        .color_count = 2,
        .colors = (Color[]){COLOR_LEMONGRASS,
              COLOR_SCIENCEBLUE}  /** BLACK = 0 and NONE = -1*/
    };

    whapi_set_apikey("XYZ");

    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    whapi_get_wallpaper_info_raw_to_file("gwjq3d", "apiwallpaperinfo.json");
    // printf("Response: %s\n", temp.str);

    whapi_search_raw(search_params, &temp);
    whapi_search_raw_to_file(search_params, "apisearch.json");
    // printf("Response: %s\n", temp.str);

    whapi_get_collections_raw(NULL, &temp);
    whapi_get_collections_raw_to_file(NULL, "apicollections.json");

    whapi_get_collections_raw("kshku", &temp);
    whapi_get_collections_raw_to_file("kshku", "apicollectionsuser.json");

    whapi_set_apikey(NULL);

    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    whapi_get_wallpaper_info_raw_to_file("gwjq3d", "wallpaperinfo.json");
    // printf("Response: %s\n", temp.str);

    whapi_search_raw(search_params, &temp);
    whapi_search_raw_to_file(search_params, "search.json");
    // printf("Response: %s\n", temp.str);

    whapi_get_collections_raw(NULL, &temp);
    whapi_get_collections_raw_to_file(NULL, "collections.json");

    whapi_get_collections_raw("kshku", &temp);
    whapi_get_collections_raw_to_file("kshku", "collectionsuser.json");

    whstr_destroy(&temp);

    Wallpaper wall;
    whapi_get_wallpaper_info("gwjq3d", &wall);
    whapi_destroy_wallpaper(&wall);

    whapi_shutdown();

    // whStr str = whstr_create();

    // whstr_setf(&str, "%dx%fy%u", -10, 0.52, 20);
    // printf("%s\n", str.str);

    // whstr_appendf(&str, "%s", "HAHAHAHAHHA");
    // printf("%s\n", str.str);

    // whstr_destroy(&str);
}
