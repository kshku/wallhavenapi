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
} whAPI;

extern whAPI whapi;

size_t write_to_whstr(void *buffer, size_t size, size_t nmemb, void *userp);

size_t write_to_file(void *data, size_t size, size_t nmemb, void *clientp);

bool reset_url(void);

bool append_query(const char *key, const char *value);

bool concat_and_set_path(const char *p, const char *q);
