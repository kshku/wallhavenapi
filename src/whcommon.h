#pragma once

#include <curl/curl.h>

#include "whapi/whstr.h"

#define WALLHAVEN_URL "https://wallhaven.cc"

#define WALLPAPER_INFO_PATH "/api/v1/w/"
#define TAG_INFO_PATH "/api/v1/tag/"
#define USER_SETTINGS_PATH "/api/v1/settings"
#define SEARCH_PATH "/api/v1/search"
#define COLLECTIONS_PATH "/api/v1/collections"

// Check pointer, and if NULL, return
#define CHECKP_RETURN(x, r, ...) \
    do {                         \
        if ((x) == NULL) {       \
            __VA_ARGS__;         \
            return r;            \
        }                        \
    } while (0)

// Check, and if fail, return
// CURLUE_OK = CURLE_OK = 0
#define CHECK_RETURN(x, r, ...) \
    do {                        \
        if ((x) != 0) {         \
            __VA_ARGS__;        \
            return r;           \
        }                       \
    } while (0)

// Check boolean and return
#define CHECKB_RETURN(x, r, ...) \
    do {                         \
        if ((x) == false) {      \
            __VA_ARGS__;         \
            return r;            \
        }                        \
    } while (0)

typedef struct whAPI {
        bool initialized;
        bool initialized_curl;

        CURL *curl;
        CURLU *url;

        whStr apikey;

        unsigned int error_code;
        ErrorCodeType error_code_type;

        response_code_handler response_code_handler;

        bool retry;
} whAPI;

extern whAPI whapi;

bool reset_url(void);

bool append_query(const char *key, const char *value);

bool concat_and_set_path(const char *p, const char *q);

bool perform_call(void);

bool setup_wallpaper_info_url(const char *id);

bool setup_search_url(const SearchParameters *params);

bool setup_tag_info_url(const char *id);

bool setup_settings_url(void);

bool setup_collections_url(const char *user_name);

bool setup_wallpaper_from_collection_url(const char *user_name, const char *id,
                                         unsigned int purity);
