#include "common.h"

int test_raw_to_file(const char *user, const char *apikey) {
    if (!whapi_initialize(true)) {
        whapi_shutdown();
        return -1;
    }

    whapi_set_response_code_handler(handler);

    whStr file = whstr_create();

    for (int i = 0; i < 2; ++i) {
        whstr_setf(&file, "wallpaper_info%d.json", i);
        TEST(whapi_get_wallpaper_info_raw_to_file(wallid, file.str),
             "test wallpaper info raw_to_file");

        for (size_t j = 0; j < (sizeof(params) / sizeof(params[0])); ++j) {
            whstr_setf(&file, "search%d_%d.json", i, j);
            TEST(whapi_search_raw_to_file(params[j], file.str),
                 "test search raw_to_file %zu", j);
        }

        whstr_setf(&file, "tag_info%d.json", i);
        TEST(whapi_get_tag_info_raw_to_file(tag_id, file.str),
             "test tag info raw_to_file");

        whstr_setf(&file, "settings%d.json", i);
        TEST(whapi_get_settings_raw_to_file(file.str),
             "test settings raw_to_file");

        whstr_setf(&file, "collections_self%d.json", i);
        TEST(whapi_get_collections_raw_to_file(NULL, file.str),
             "test collections self raw_to_file");

        whstr_setf(&file, "collections_user%d.json", i);
        TEST(whapi_get_collections_raw_to_file(test_user_name, file.str),
             "test collections user raw_to_file");

        whstr_setf(&file, "wallpaper_from_collection_self%d.json", i);
        TEST(whapi_get_wallpapers_from_collection_raw_to_file(
                 user, 1686751, PURITY_ALL, file.str),
             "test collection wallpapers self raw_to_file");

        whstr_setf(&file, "wallpaper_from_collection_user%d.json", i);
        TEST(whapi_get_wallpapers_from_collection_raw_to_file(
                 test_user_name, test_user_collection_id, PURITY_ALL, file.str),
             "test collection wallpapers user raw_to_file");

        whapi_set_apikey(apikey);
    }

    whapi_shutdown();
}
