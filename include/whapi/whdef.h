#pragma once

#include <stdbool.h>
#include <stddef.h>

// Os detection
#if defined(__gnu_linux__) || defined(__linux__)
    #define WH_OS_LINUX
#elif defined(_WIN32) || defined(_WIN64)
    #define WH_OS_WINDOWS
#elif defined(macintosh) || defined(Macintosh) \
    || (defined(__APPLE__) && defined(__MACH__))
    #define WH_OS_MAC
#else
    #error "Don't know whether works!"
#endif

// Export macro
#if defined(WH_STATIC)
    #define WH_API
#else
    #ifdef WH_EXPORT
        #if defined(WH_OS_LINUX) || defined(WH_OS_MAC)
            #define WH_API __attribute__((visibility("default")))
        #elif defined(WH_OS_WINDOWS)
            #define WH_API __declspec(dllexport)
        #else
            #error "Should not reach here"
        #endif
    #else
        #if defined(WH_OS_LINUX) || defined(WH_OS_MAC)
            #define WH_API
        #elif defined(WH_OS_WINDOWS)
            #define WH_API __declspec(dllimport)
        #else
            #error "Should not reach here"
        #endif
    #endif
#endif

/**
 * @brief Error code type.
 */
typedef enum ErrorCodeType {
    ERROR_CODE_TYPE_NONE = 0, /**< No error */
    ERROR_CODE_TYPE_WALLHAVEN, /**< Error code from wallhaven */
    ERROR_CODE_TYPE_CURL, /**< Error code from curl (Check CURLcode) */
    ERROR_CODE_TYPE_URL /**< Error code from curl url (Check CURLUcode) */
} ErrorCodeType;

/**
 * @brief Wallhaven's error codes.
 */
typedef enum WallhavenErrorCode {
    WALLHAVEN_OK = 0, /**< No errors */
    WALLHAVEN_NO_API_KEY, /**< API key must be provided */
    WALLHAVEN_NULL_ID, /**< Id should not be NULL */
    WALLHAVEN_NULL_USER_NAME, /**< User name should not be NULL */
    WALLHAVEN_ALLOCATION_FAIL, /**< Failed malloc or other allocation functions
                                */
    WALLHAVEN_MULTIPLE_INITIALIZE, /**< Initializing wallhaven multiple times */
    WALLHAVEN_CURL_INIT_FAILED, /**< Failed to initialize curl */
    WALLHAVEN_SORTING_MUST_BE_TOPLIST, /**< To use toplist range, sorting must
                                          be set to toplist */
    WALLHAVEN_CANNOT_COMBINE_ID, /**< Cannot combine the exact id search with
                                    other params */
    WALLHAVEN_INVALID_SEED, /**< Seed should be [a-zA-Z0-9]{6} */
    WALLHAVEN_OPEN_FILE_FAILED, /**< Failed to open file for writing */
    WALLHAVEN_PARSING_FAILED, /**< Failed to parse the json data */
    WALLHAVEN_UNAUTHORIZED_ACCESS, /**< Unauthorized access response code */
    WALLHAVEN_NOTHING_IS_HERE, /**< Page not found response code */
    WALLHAVEN_API_CALL_LIMIT_EXCEED, /**< API call limit exceeded response */
    WALLHAVEN_BAD_REQUEST, /**< Bad request response */
    WALLHAVEN_UNKNOWN_ERROR, /**< Unknown error */
    WALLHAVEN_INVALID_WALLPAPER, /**< Invalid wallpaper is given to install */
} WallhavenErrorCode;

/**
 * @brief Response code handler type.
 *
 * @param response_code The response code
 *
 * @return Return true to retry, false to quit.
 */
typedef bool (*response_code_handler)(unsigned int response_code);

/**
 * @brief Type of image.
 */
typedef enum ImageType {
    IMAGE_TYPE_NONE = 0, /**< None */
    IMAGE_TYPE_PNG, /**< PNG images */
    IMAGE_TYPE_JPG, /**< JPG or JPEG images */
    IMAGE_TYPE_JPEG = IMAGE_TYPE_JPG /**< JPG or JPEG images */
} ImageType;

/**
 * @brief Search query.
 */
typedef struct Query {
        size_t id; /**< Exact tag serach (tag id) */
        const char
            *tags; /**< Fuzzy search tag/keyword ('tagname' - search fuzzily for
                      a tag/keyword, '-tagname' - exclude a tag/keyword, '+tag1
                      +tag2' - must have tag1 and tag2, '+tag1 -tag2' - must
                      have tag1 and NOT tag2) */
        const char *user_name; /**< User uploads */
        ImageType type; /**< Image type */
        const char *like; /**< Wallpapers with similar tags */
} Query;

/**
 * @brief Wallpaper Category (bitflags).
 */
typedef enum Category {
    CATEGORY_NONE = 0, /**< None */
    CATEGORY_GENERAL = 1 << 0, /**< General category */
    CATEGORY_ANIME = 1 << 1, /**< Anime category */
    CATEGORY_PEOPLE = 1 << 2, /**< People category */
    CATEGORY_ALL = CATEGORY_ANIME | CATEGORY_GENERAL
                 | CATEGORY_PEOPLE, /**< All categories */
} Category;

/**
 * @brief Wallpaper purity (bitflags).
 *
 * https://wallhaven.cc/rules#Purity
 */
typedef enum Purity {
    PURITY_NONE = 0, /**< None */
    PURITY_SFW = 1 << 0, /**< Safe for work */
    PURITY_SKETCHY = 1 << 1, /**< Sketchy */
    PURITY_NSFW = 1 << 2, /**< Not safe for work */
    PURITY_ALL = PURITY_SFW | PURITY_NSFW | PURITY_SKETCHY, /**< Purity all */
} Purity;

/**
 * @brief Sorting method.
 */
typedef enum Sorting {
    SORTING_NONE = 0, /**< None */
    SORTING_DATE_ADDED, /**< Added date */
    SORTING_RELEVANCE, /**< Relevance */
    SORTING_RANDOM, /**< Random */
    SORTING_VIEWS, /**< Views */
    SORTING_FAVORITES, /**< Favorites */
    SORTING_TOPLIST /**< Toplist */
} Sorting;

/**
 * @brief The sorting order.
 */
typedef enum Order {
    ORDER_NONE = 0, /**< None */
    ORDER_DESCENDING, /**< Descending order */
    ORDER_ASCENDING, /**< Ascending order */
} Order;

/**
 * @brief Toplist range.
 */
typedef enum TopRange {
    TOPRANGE_NONE = 0, /**< None */
    TOPRANGE_1D, /**< 1 day */
    TOPRANGE_3D, /**< 3 days */
    TOPRANGE_1W, /**< 1 week */
    TOPRANGE_1M, /**< 1 month */
    TOPRANGE_3M, /**< 3 months */
    TOPRANGE_6M, /**< 6 months */
    TOPRANGE_1Y, /**< 1 year */
} TopRange;

/**
 * @brief Resolution of image.
 */
typedef struct Resolution {
        unsigned int width, height;
} Resolution;

/**
 * @brief Ratio of image.
 */
typedef Resolution Ratio;

/**
 * @brief Color of image.
 */
typedef enum Color {
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
} Color;

/**
 * @brief Parameters for searching.
 */
typedef struct SearchParameters {
        Query q; /**< Search query */
        unsigned int categories; /**< Categories mask */
        unsigned int purity; /**< Purity mask */
        Sorting sorting; /**< Sorting method */
        Order order; /**< Sorting order */
        TopRange toprange; /**< Toplist range */
        Resolution atleast_resolution; /**< Minimum resolution */
        size_t exact_resolution_count; /**< Number of exact resolutions */
        const Resolution *exact_resolutions; /**< Array of exact resolutions */
        size_t ratio_count; /**< Number of ratios */
        const Ratio *ratios; /**< Array of ratios */
        size_t color_count; /**< Number of colors */
        const Color *colors; /**< Array of colors */
        size_t page; /**< Page number */
        char seed[7]; /** Seed */
} SearchParameters;
