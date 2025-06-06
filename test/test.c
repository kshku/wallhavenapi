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
        .colors = (Color[]){COLOR_LEMONGRASS, COLOR_SCIENCEBLUE}
    };

    whapi_set_apikey("XYZ");

    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    // printf("Response: %s\n", temp.str);

    whstr_clear(&temp);

    whapi_search_raw(search_params, &temp);
    // printf("Response: %s\n", temp.str);

    whstr_clear(&temp);

    whapi_get_collections_raw(NULL, &temp);

    whstr_clear(&temp);

    whapi_get_collections_raw("kshku", &temp);

    whstr_clear(&temp);

    whapi_set_apikey(NULL);

    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    // printf("Response: %s\n", temp.str);

    whstr_clear(&temp);

    whapi_search_raw(search_params, &temp);
    // printf("Response: %s\n", temp.str);

    whstr_clear(&temp);

    whapi_get_collections_raw(NULL, &temp);

    whstr_clear(&temp);

    whapi_get_collections_raw("kshku", &temp);

    whstr_destroy(&temp);

    whapi_shutdown();

    whStr str = whstr_create();

    whstr_setf(&str, "%dx%fy%u", -10, 0.52, 20);
    printf("%s\n", str.str);

    whstr_appendf(&str, "%s", "HAHAHAHAHHA");
    printf("%s\n", str.str);

    whstr_destroy(&str);
}
