#include "common.h"

void print_tag_info(Tag *tag) {
    // print everything in tag in order except json
    printf("id: \"%zu\"\n", tag->id);
    printf("name: \"%s\"\n", tag->name);
    printf("alias: \"%s\"\n", tag->alias ? tag->alias : "NULL");
    printf("category_id: \"%zu\"\n", tag->category_id);
    printf("category: \"%s\"\n", tag->category);
    printf("purity: \"%u\"\n", tag->purity);
    printf("created_at: \"%s\"\n", tag->created_at);

    printf("\n");
}

void print_wallpaper(Wallpaper *wallpaper) {
    // print everything in wallpaper in order except json
    printf("Id: \"%s\"\n", wallpaper->id);
    printf("url: \"%s\"\n", wallpaper->url);
    printf("short_url: \"%s\"\n", wallpaper->short_url);
    printf("uploader:\n\tuser_name: \"%s\"\n", wallpaper->uploader.user_name);
    printf("\tgroup: \"%s\"\n", wallpaper->uploader.group);
    printf("\tavatar200px: \"%s\"\n", wallpaper->uploader.avatar200px);
    printf("\tavatar128px: \"%s\"\n", wallpaper->uploader.avatar128px);
    printf("\tavatar32px: \"%s\"\n", wallpaper->uploader.avatar32px);
    printf("\tavatar20px: \"%s\"\n", wallpaper->uploader.avatar20px);
    printf("views: \"%zu\"\n", wallpaper->views);
    printf("favorites: \"%zu\"\n", wallpaper->favorites);
    printf("source: \"%s\"\n", wallpaper->source);
    printf("purity: \"%u\"\n", wallpaper->purity);
    printf("category: \"%u\"\n", wallpaper->category);
    printf("resolution: \"%ux%u\"\n", wallpaper->resolution.width,
           wallpaper->resolution.height);
    printf("ratio: \"%.2f\"\n", wallpaper->ratio);
    printf("file_size: \"%zu bytes\"\n", wallpaper->file_size);
    printf("type: \"%u\"\n", wallpaper->type);
    printf("created_at: \"%s\"\n", wallpaper->created_at);
    printf("color_count: \"%zu\"\n", wallpaper->color_count);
    for (size_t i = 0; i < wallpaper->color_count; ++i) {
        printf("color[%zu]: \"%06x\"\n", i, wallpaper->colors[i]);
    }
    printf("path: \"%s\"\n", wallpaper->path);
    printf("thumbs:\n\tlarge: \"%s\"\n\toriginal: \"%s\"\n\tsmall: \"%s\"\n",
           wallpaper->thumbs.large_size, wallpaper->thumbs.original_size,
           wallpaper->thumbs.small_size);
    printf("tag_count: \"%zu\"\n", wallpaper->tag_count);
    for (size_t i = 0; i < wallpaper->tag_count; ++i) {
        printf("tag[%zu]:\n", i);
        print_tag_info(&wallpaper->tags[i]);
    }

    printf("\n");
}

void print_search_result(SearchResult *search_result) {
    // print everything in SearchResult in order except json
    printf("wallpaper_count: \"%zu\"\n", search_result->wallpaper_count);
    for (size_t i = 0; i < search_result->wallpaper_count; ++i) {
        printf("wallpaper[%zu]:\n", i);
        print_wallpaper(&search_result->wallpapers[i]);
        printf("Downloading %s\n", search_result->wallpapers[i].path);
        if (
#ifdef WH_OS_WINDOWS
            whapi_download_wallpaper(&search_result->wallpapers[i],
                                     "..\\Downloads")
#else
            whapi_download_wallpaper(&search_result->wallpapers[i],
                                     "../Downloads")
#endif
        )
            printf("Done!\n");
        else printf("Something went wrong!\n");
        printf("\n");
    }

    printf("current_page: \"%zu\"\n", search_result->current_page);
    printf("last_page: \"%zu\"\n", search_result->last_page);
    printf("per_page: \"%zu\"\n", search_result->per_page);
    printf("total: \"%zu\"\n", search_result->total);
    printf("seed: \"%s\"\n", search_result->seed);
    printf("type: \"%s\"\n", search_result->type == NORMAL_SEARCH
                                 ? "NORMAL_SEARCH"
                                 : "EXACT_TAG_SEARCH");
    if (search_result->type == NORMAL_SEARCH) {
        printf("query: \"%s\"\n", search_result->query);
    } else {
        printf("tag id: \"%zu\"\n", search_result->id);
        printf("tag: \"%s\"\n", search_result->tag);
    }

    printf("\n");
}

void print_settings(Settings *settings) {
    // print everything in settings in order except json
    printf("thumb_size: \"%s\"\n", settings->thumb_size);
    printf("per_page: \"%zu\"\n", settings->per_page);
    printf("purity: \"%u\"\n", settings->purity);
    printf("categories: \"%u\"\n", settings->categories);
    printf("toplist_range: \"%u\"\n", settings->toplist_range);
    printf("resolution_count: \"%zu\"\n", settings->resolution_count);
    for (size_t i = 0; i < settings->resolution_count; ++i) {
        printf("resolution[%zu]: \"%ux%u\"\n", i,
               settings->resolutions[i].width, settings->resolutions[i].height);
    }
    printf("aspect_ratio_count: \"%zu\"\n", settings->aspect_ratio_count);
    for (size_t i = 0; i < settings->aspect_ratio_count; ++i) {
        printf("aspect_ratio[%zu]: \"%ux%u\"\n", i,
               settings->aspect_ratios[i].width,
               settings->aspect_ratios[i].height);
    }
    printf("tag_blacklist_count: \"%zu\"\n", settings->tag_blacklist_count);
    for (size_t i = 0; i < settings->tag_blacklist_count; ++i) {
        printf("tag_blacklist[%zu]: \"%s\"\n", i, settings->tag_blacklist[i]);
    }
    printf("user_blacklist_count: \"%zu\"\n", settings->user_blacklist_count);
    for (size_t i = 0; i < settings->user_blacklist_count; ++i) {
        printf("user_blacklist[%zu]: \"%s\"\n", i, settings->user_blacklist[i]);
    }

    printf("\n");
}

void print_collection(Collection *collection) {
    // print everything in collection in order
    printf("id: \"%zu\"\n", collection->id);
    printf("label: \"%s\"\n", collection->label);
    printf("views: \"%zu\"\n", collection->views);
    printf("public: \"%s\"\n", collection->public ? "true" : "false");
    printf("count: \"%zu\"\n", collection->count);

    printf("\n");
}

void print_collections(Collections *collections) {
    // print everything in collections in order except json
    printf("collection_count: \"%zu\"\n", collections->collection_count);
    for (size_t i = 0; i < collections->collection_count; ++i) {
        printf("collection[%zu]:\n", i);
        print_collection(&collections->collections[i]);
    }

    printf("\n");
}

int test_parsed(const char *user, const char *apikey) {
    if (!whapi_initialize(true)) {
        whapi_shutdown();
        return -1;
    }

    whapi_set_response_code_handler(handler);

    for (int k = 0; k < 2; ++k) {
        Wallpaper wallpaper;
        TEST(whapi_get_wallpaper_info(wallid, &wallpaper),
             "test wallpaper info parsed");
        print_wallpaper(&wallpaper);
        whapi_destroy_wallpaper(&wallpaper);

        SearchResult search_result;
        for (size_t i = 0; i < (sizeof(params) / sizeof(params[0])); ++i) {
            TEST(whapi_search(params[i], &search_result),
                 "test search parsed %zu", i);
            print_search_result(&search_result);
            whapi_destroy_search_result(&search_result);
        }

        Tag tag;
        TEST(whapi_get_tag_info(tag_id, &tag), "test tag info parsed");
        print_tag_info(&tag);
        whapi_destroy_tag(&tag);

        Settings settings;
        TEST(whapi_get_settings(&settings), "test settings parsed");
        print_settings(&settings);
        whapi_destroy_settings(&settings);

        Collections collections;
        TEST(whapi_get_collections(NULL, &collections),
             "test collections self parsed");
        print_collections(&collections);
        whapi_destroy_collections(&collections);

        TEST(whapi_get_collections(test_user_name, &collections),
             "test collections user parsed");
        print_collections(&collections);
        whapi_destroy_collections(&collections);

        TEST(whapi_get_wallpapers_from_collection(user, 1686751, PURITY_ALL,
                                                  &search_result),
             "test collection wallpapers self parsed");
        print_search_result(&search_result);
        whapi_destroy_search_result(&search_result);

        TEST(whapi_get_wallpapers_from_collection(test_user_name,
                                                  test_user_collection_id,
                                                  PURITY_ALL, &search_result),
             "test collection wallpapers user parsed");
        print_search_result(&search_result);
        whapi_destroy_search_result(&search_result);

        whapi_set_apikey(apikey);
    }

    whapi_shutdown();

    return 0;
}
