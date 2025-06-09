#pragma once

#include <stdio.h>
#include <whapi/whapi.h>

#define STRINGIFY(x) #x

static const char *wallhaven_error_code_string(WallhavenErrorCode code) {
    switch (code) {
        case WALLHAVEN_OK:
            return STRINGIFY(WALLHAVEN_OK);
        case WALLHAVEN_NO_API_KEY:
            return STRINGIFY(WALLHAVEN_NO_API_KEY);
        case WALLHAVEN_NULL_ID:
            return STRINGIFY(WALLHAVEN_NULL_ID);
        case WALLHAVEN_NULL_USER_NAME:
            return STRINGIFY(WALLHAVEN_NULL_USER_NAME);
        case WALLHAVEN_ALLOCATION_FAIL:
            return STRINGIFY(WALLHAVEN_ALLOCATION_FAIL);
        case WALLHAVEN_MULTIPLE_INITIALIZE:
            return STRINGIFY(WALLHAVEN_MULTIPLE_INITIALIZE);
        case WALLHAVEN_CURL_INIT_FAILED:
            return STRINGIFY(WALLHAVEN_CURL_INIT_FAILED);
        case WALLHAVEN_SORTING_MUST_BE_TOPLIST:
            return STRINGIFY(WALLHAVEN_SORTING_MUST_BE_TOPLIST);
        case WALLHAVEN_CANNOT_COMBINE_ID:
            return STRINGIFY(WALLHAVEN_CANNOT_COMBINE_ID);
        case WALLHAVEN_INVALID_SEED:
            return STRINGIFY(WALLHAVEN_INVALID_SEED);
        case WALLHAVEN_OPEN_FILE_FAILED:
            return STRINGIFY(WALLHAVEN_OPEN_FILE_FAILED);
        case WALLHAVEN_PARSING_FAILED:
            return STRINGIFY(WALLHAVEN_PARSING_FAILED);
        case WALLHAVEN_UNAUTHORIZED_ACCESS:
            return STRINGIFY(WALLHAVEN_UNAUTHORIZED_ACCESS);
        case WALLHAVEN_NOTHING_IS_HERE:
            return STRINGIFY(WALLHAVEN_NOTHING_IS_HERE);
        case WALLHAVEN_API_CALL_LIMIT_EXCEED:
            return STRINGIFY(WALLHAVEN_API_CALL_LIMIT_EXCEED);
        case WALLHAVEN_UNKNOWN_ERROR:
            return STRINGIFY(WALLHAVEN_UNKNOWN_ERROR);
        case WALLHAVEN_BAD_REQUEST:
            return STRINGIFY(WALLHAVEN_BAD_REQUEST);
        default:
            break;
    }
    return NULL;
}

#define TEST(x, task, ...)                                          \
    do {                                                            \
        if (!(x)) {                                                 \
            unsigned int code, type;                                \
            whapi_get_last_error_code(&code, &type);                \
            printf("Failed \"" task "\"\n", ##__VA_ARGS__);         \
            printf("Error type: %u, Error code: %u\n", code, type); \
            if (type == ERROR_CODE_TYPE_WALLHAVEN)                  \
                printf("%s\n", wallhaven_error_code_string(code));  \
            printf("\n");                                           \
        } else {                                                    \
            printf("Passed \"" task "\"\n\n", ##__VA_ARGS__);       \
        }                                                           \
    } while (0)

static SearchParameters params[] = {
    {.q = {.id = 13, .tags = "nature"}},

    {.q = {.id = 13},
     .atleast_resolution = (Resolution){.width = 1920, .height = 1080},
     .categories = CATEGORY_GENERAL,
     .purity = PURITY_SFW},

    {.colors = (Color[]){COLOR_ROSEWOOD, COLOR_APPLEGREEN},
     .color_count = 2,
     .exact_resolution_count = 2,
     .exact_resolutions = (Resolution[]){{.width = 1920, .height = 1080},
                                         {.width = 600, .height = 600}}},

    {.q = {.tags = "nature +mountain -city", .type = IMAGE_TYPE_PNG},
     .ratio_count = 1,
     .ratios = (Ratio[]){{.width = 16, .height = 9}}},

    {.q = {.tags = "nature", .like = "pkp1vp", .type = IMAGE_TYPE_PNG},
     .ratio_count = 1,
     .ratios = (Ratio[]){{.width = 16, .height = 9}}},

    {.order = ORDER_DESCENDING,
     .sorting = SORTING_VIEWS,
     .page = 3,
     .seed = "xyzAB1"},

    {.q = {.tags = "nature"},
     .sorting = SORTING_TOPLIST,
     .toprange = TOPRANGE_1W},

    {0},

    {.colors = (Color[]){COLOR_CRIMSONRED}, .color_count = 1},

    {.page = 99, .sorting = SORTING_RANDOM},

    {.page = 99999, .sorting = SORTING_RANDOM},

    {.categories = CATEGORY_ALL, .purity = PURITY_ALL},

    {.q = {.like = "w8ymjq"}},

    {.q = {.type = IMAGE_TYPE_JPG}},

    {.q = {.tags = "city lights"},
     .atleast_resolution = {.width = 3840, .height = 2160}},

    {.q = {.like = "w8ymjq", .type = IMAGE_TYPE_PNG}},

    {.sorting = SORTING_TOPLIST,
     .toprange = TOPRANGE_1M,
     .ratios = (Ratio[]){{.width = 21, .height = 9}},
     .ratio_count = 1},

    {.q = {.user_name = "AprilFlowers"}}
};

static const char *wallid = "w8ymjq";
static const size_t tag_id = 12162;

static const char *test_user_name = "SOME_USER";
static const size_t test_user_collection_id =
    0;  // Id of one of that user's collection

#ifdef WH_WINDOWS
    #include <windows.h>
#else
    #include <unistd.h>
#endif

static void sleep_ms(int milliseconds) {
#ifdef WH_WINDOWS
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

static bool handler(unsigned int response) {
    printf("Response code: %u\n", response);
    if (response == 429) {
        // sleep 15 seconds
        sleep_ms(1000 * 15);
        return true;
    }

    return false;
}
