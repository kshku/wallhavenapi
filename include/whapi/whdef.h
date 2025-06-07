#pragma once

#include <stdbool.h>
#include <stddef.h>

#if defined(__gnu_linux__) || defined(__linux__)
    #define WH_OS_LINUX
#elif defined(_WIN64)
    #define WH_OS_WINDOWS
#else
    #error "Don't know whether works!"
#endif

#ifdef WH_EXPORT
    #if defined(WH_OS_LINUX)
        #define WH_API __attribute__((visibility("default")))
    #elif defined(WH_OS_WINDOWS)
        #define WH_API __declspec(dllexport)
    #else
        #error "Should not reach here"
    #endif
#else
    #if defined(WH_OS_LINUX)
        #define WH_API
    #elif defined(WH_OS_WINDOWS)
        #define WH_API __declspec(dllimport)
    #else
        #error "Should not reach here"
    #endif
#endif

typedef enum ErrorCodeType {
    ERROR_CODE_TYPE_NONE = 0,
    ERROR_CODE_TYPE_WALLHAVEN,
    ERROR_CODE_TYPE_CURL,
    ERROR_CODE_TYPE_URL
} ErrorCodeType;

typedef enum WallhavenErrorCode {
    WALLHAVEN_OK = 0,
    WALLHAVEN_NO_API_KEY,
    WALLHAVEN_NULL_ID,
    WALLHAVEN_NULL_USER_NAME,
    WALLHAVEN_ALLOCATION_FAIL,
    WALLHAVEN_MULTIPLE_INITIALIZE,
    WALLHAVEN_CURL_INIT_FAILED,
    WALLHAVEN_SORTING_MUST_BE_TOPLIST,
    WALLHAVEN_CANNOT_COMBINE_ID,
    WALLHAVEN_INVALID_SEED,
    WALLHAVEN_OPEN_FILE_FAILED,
    WALLHAVEN_PARSING_FAILED,
} WallhavenErrorCode;

/**
 * @brief Response code handler type.
 *
 * @param response_code The response code
 *
 * @return Return true to retry, false to quit.
 */
typedef bool (*response_code_handler)(unsigned int response_code);

typedef enum ImageType {
    IMAGE_TYPE_NONE = 0,
    IMAGE_TYPE_PNG,
    IMAGE_TYPE_JPG,
    IMAGE_TYPE_JPEG = IMAGE_TYPE_JPG
} ImageType;

/**
 * @brief Structure for the searching options.
 */
typedef struct Query {
        unsigned int id;
        const char *tags;
        const char *user_name;
        ImageType type;
        const char *like;
} Query;

typedef enum Category {
    CATEGORY_NONE = 0,
    CATEGORY_GENERAL = 1 << 0,
    CATEGORY_ANIME = 1 << 1,
    CATEGORY_PEOPLE = 1 << 2
} Category;

typedef enum Purity {
    PURITY_NONE = 0,
    PURITY_SFW = 1 << 0,
    PURITY_SKETCHY = 1 << 1,
    PURITY_NSFW = 1 << 2
} Purity;

typedef enum Sorting {
    SORTING_NONE = 0,
    SORTING_DATE_ADDED,
    SORTING_RELEVANCE,
    SORTING_RANDOM,
    SORTING_VIEWS,
    SORTING_FAVORITES,
    SORTING_TOPLIST
} Sorting;

typedef enum Order { ORDER_NONE = 0, ORDER_DESCENDING, ORDER_ASCENDING } Order;

typedef enum TopRange {
    TOPRANGE_NONE = 0,
    TOPRANGE_1D,
    TOPRANGE_3D,
    TOPRANGE_1W,
    TOPRANGE_1M,
    TOPRANGE_3M,
    TOPRANGE_6M,
    TOPRANGE_1Y,
} TopRange;

typedef struct Resolution {
        unsigned int width, height;
} Resolution;

typedef enum Color {
    COLOR_NONE = -1,
    COLOR_ROSEWOOD = 0x660000,
    COLOR_CRIMSONRED = 0x990000,
    COLOR_ROSSOCORSA = 0xcc0000,
    COLOR_PERSIANRED = 0xcc3333,
    COLOR_DARKPINK = 0xea4c88,
    COLOR_WARMPURPLE = 0x993399,
    COLOR_EMINENCE = 0x663399,
    COLOR_BLUEBERRY = 0x333399,
    COLOR_SCIENCEBLUE = 0x0066cc,
    COLOR_PACIFICBLUE = 0x0099cc,
    COLOR_DOWNY = 0x66cccc,
    COLOR_APPLEGREEN = 0x77cc33,
    COLOR_VENOMGREEN = 0x669900,
    COLOR_GREENLEAF = 0x336600,
    COLOR_GREENYBROWN = 0x666600,
    COLOR_BROWNYELLOW = 0x999900,
    COLOR_BIRDFLOWER = 0xcccc33,
    COLOR_ARTYCLICKYELLOW = 0xffff00,
    COLOR_SUNGLOW = 0xffcc33,
    COLOR_ORANGEPEEL = 0xff9900,
    COLOR_BLAZEORANGE = 0xff6600,
    COLOR_TERRACOTTA = 0xcc6633,
    COLOR_WOOD = 0x996633,
    COLOR_NUTMEGWOOD = 0x663300,
    COLOR_BLACK = 0x000000,
    COLOR_LEMONGRASS = 0x999999,
    COLOR_PASTELGREY = 0xcccccc,
    COLOR_WHITE = 0xffffff,
    COLOR_GUNPOWDER = 0x424153,
}

Color;

typedef Resolution Ratio;

typedef struct SearchParameters {
        Query q;
        unsigned int categories;
        unsigned int purity;
        Sorting sorting;
        Order order;
        TopRange toprange;
        Resolution atleast_resolution;
        size_t exact_resolution_count;
        const Resolution *exact_resolutions;
        size_t ratio_count;
        const Ratio *ratios;
        size_t color_count;
        const Color *colors;
        unsigned int page;
        const char seed[7];
} SearchParameters;
