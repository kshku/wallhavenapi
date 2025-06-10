#include "common.h"

int test_raw(const char *user, const char *apikey) {
    if (!whapi_initialize(true)) {
        whapi_shutdown();
        return -1;
    }

    whapi_set_response_code_handler(handler);

    whStr res = whstr_create();

    for (int i = 0; i < 2; ++i) {
        TEST(whapi_get_wallpaper_info_raw(wallid, &res),
             "test wallpaper info raw");

        for (size_t i = 0; i < (sizeof(params) / sizeof(params[0])); ++i)
            TEST(whapi_search_raw(params[i], &res), "test search raw %zu", i);

        TEST(whapi_get_tag_info_raw(tag_id, &res), "test tag info raw");

        TEST(whapi_get_settings_raw(&res), "test settings raw");

        TEST(whapi_get_collections_raw(NULL, &res),
             "test collections self raw");

        TEST(whapi_get_collections_raw(test_user_name, &res),
             "test collections user raw");

        TEST(whapi_get_wallpapers_from_collection_raw(user, 1686751, PURITY_ALL,
                                                      &res),
             "test collection wallpapers self raw");

        TEST(whapi_get_wallpapers_from_collection_raw(
                 test_user_name, test_user_collection_id, PURITY_ALL, &res),
             "test collection wallpapers user raw");

        whapi_set_apikey(apikey);
    }

    whstr_destroy(&res);

    whapi_shutdown();

    return 0;
}
