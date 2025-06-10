#pragma once

#include "whapi/whcore.h"
#include "whapi/whstr.h"

typedef struct cJSON cJSON;  // Internal

/**
 * @brief Tag info.
 */
typedef struct Tag {
        size_t id; /**< Tag id */
        const char *name; /**< Tag name */
        const char *alias; /**< Tag aliases */
        size_t category_id; /**< Tag category id */
        const char *category; /**< Category */
        Purity purity; /**< Purtiy */
        const char *created_at; /**< created timestamp */
        cJSON *json; /**< Internal */
} Tag;

/**
 * @brief Settings info.
 */
typedef struct Settings {
        const char *thumb_size; /**< Thumb size */
        size_t per_page; /**< Images per page */
        unsigned int purity; /**< Purity */
        unsigned int categories; /**< Categories */

        TopRange toplist_range; /**< Toplist range */

        size_t resolution_count; /**< Number of resoulutions */
        Resolution *resolutions; /**< Array of resolutions */

        size_t aspect_ratio_count; /**< Number of aspect ratio */
        Ratio *aspect_ratios; /**< Array of aspect ratio */

        size_t tag_blacklist_count; /**< Number of blacklisted tags */
        const char **tag_blacklist; /**< Array of blacklisted tags */

        size_t user_blacklist_count; /**< Number of blacklisted users */
        const char **user_blacklist; /**< Array of blacklisted users */

        cJSON *json; /**< Internal */
} Settings;

/**
 * @brief Image thumbs.
 */
typedef struct Thumbs {
        const char *large_size;
        const char *original_size;
        // rpcndr.h has #define small char which is conflicting with the Thumbs
        // struct member small in windows
        const char *small_size;
} Thumbs;

/**
 * @brief User info.
 */
typedef struct User {
        const char *user_name; /**< User name */
        const char *group; /**< Group to which user belongs to */
        const char *avatar200px; /**< Avatar image 200px */
        const char *avatar128px; /**< Avatar image 128px */
        const char *avatar32px; /**< Avatar image 32px */
        const char *avatar20px; /**< Avatar image 20px */
} User;

/**
 * @brief Wallpaper info.
 */
typedef struct Wallpaper {
        const char *id; /**< Wallpaper id */
        const char *url; /**< Wallpaper url */
        const char *short_url; /**< Wallpaper short url */

        User uploader; /**< The uploader */

        size_t views; /**< Number of views */
        size_t favorites; /**< Favorites */

        const char *source; /**< Source */

        Purity purity; /**< Purity */
        Category category; /**< Category */

        Resolution resolution; /**< Resolution */
        float ratio; /**< Ratio in fraction */

        size_t file_size; /**< Image size */
        ImageType type; /**< Image typ */

        const char *created_at; /**< Created at time stamp */

        size_t color_count; /**< Number of colors */
        Color *colors; /**< Array of colors */

        const char *path; /**< Image path */

        Thumbs thumbs; /**< Thumbs */

        size_t tag_count; /**< Number of tags */
        Tag *tags; /**< Array of tags */

        cJSON *json; /**< Internal */
} Wallpaper;

/**
 * @brief Search result.
 */
typedef struct SearchResult {
        size_t wallpaper_count; /**< Number of wallpapers */
        Wallpaper *wallpapers; /**< Array of wallpapers */

        size_t current_page; /**< Current page */
        size_t last_page; /**< Last page */
        size_t per_page; /**< Per page wallpapers */
        size_t total; /**< Total wallpapers */
        const char *seed; /**< Seed (if sorting is random) */

        struct {
                union {
                        const char *query; /**< Query */

                        struct {
                                size_t id; /**< Tag id in case of exact tag
                                              search */
                                const char *tag; /**< Tag name in case of exact
                                                    tag search */
                        };
                };

                enum {
                    NORMAL_SEARCH,
                    EXACT_TAG_SEARCH
                } type; /**< Search type Normal or exact tag search */
        };

        cJSON *json; /**< Internal */
} SearchResult;

/**
 * @brief Collection info.
 */
typedef struct Collection {
        size_t id; /**< Collection id */
        const char *label; /**< Collection name */
        size_t views; /**< Number of views */
        bool public; /**< Is public? */
        size_t count; /**< Number of wallpapers in the collection */
} Collection;

/**
 * @brief Collections, just array of Collection.
 */
typedef struct Collections {
        size_t collection_count; /**< Number of collection */
        Collection *collections; /**< Array of collections */

        cJSON *json; /**< Internal */
} Collections;

/**
 * @brief Get the wallpaper info.
 *
 * Uses apikey if set.
 *
 * @param id The wallpaper id
 * @param wallpaper Pointer to wallpaper
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_wallpaper_info(const char *id, Wallpaper *wallpaper);

/**
 * @brief Search.
 *
 * Uses apikey if set.
 *
 * @param params SearchParameters
 * @param search_result Pointer to SearchResult
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_search(SearchParameters params, SearchResult *search_result);

/**
 * @brief Get the tag info.
 *
 * @param id Tag id
 * @param tag Pointer to Tag
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_tag_info(size_t id, Tag *tag);

/**
 * @brief Get the user settings.
 *
 * Fails if apikey is not set.
 *
 * @param settings Pointer to Settings
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_settings(Settings *settings);

/**
 * @brief List the collections.
 *
 * "Only collections that are public will be accessible to other users. When
 * authenticated, you are able to view all of your own collections"
 *
 * If user_name is NULL, then collection of the user is listed (apikey is must).
 *
 * @param user_name The username from which to list collections
 * @param collections Pointer to Collections
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_collections(const char *user_name,
                                  Collections *collections);

/**
 * @brief Get the wallpapers from a collection.
 *
 * If apikey is set, it is used. To access private collections, need apikey.
 *
 * @param user_name Username (even if your own collection)
 * @param id The collection id
 * @param purity The purity filter
 * @param search_result Pointer to SeachResult
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_wallpapers_from_collection(const char *user_name,
                                                 size_t id, unsigned int purity,
                                                 SearchResult *search_result);

/**
 * @brief Destroy the Tag.
 *
 * @param tag Pointer to tag
 */
WH_API void whapi_destroy_tag(Tag *tag);

/**
 * @brief Destroy the Settings.
 *
 * @param settings Pointer to settings
 */
WH_API void whapi_destroy_settings(Settings *settings);

/**
 * @brief Destroy the wallpaper.
 *
 * @param wallpaper Pointer to wallpaper
 */
WH_API void whapi_destroy_wallpaper(Wallpaper *wallpaper);

/**
 * @brief Destroy the search result.
 *
 * @param search_result Pointer to the search result
 */
WH_API void whapi_destroy_search_result(SearchResult *search_result);

/**
 * @brief Destroy the collections.
 *
 * @param collections Pointer to the collections
 */
WH_API void whapi_destroy_collections(Collections *collections);
