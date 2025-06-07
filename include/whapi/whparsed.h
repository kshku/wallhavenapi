#pragma once

#include "whapi/whcore.h"
#include "whapi/whstr.h"

typedef struct cJSON cJSON;

typedef struct Tag {
        size_t id;
        const char *name;
        const char *alias;
        size_t category_id;
        const char *category;
        Purity purity;
        const char *created_at;
        cJSON *json;
} Tag;

typedef struct Settings {
        const char *thumb_size;
        unsigned int per_page;
        unsigned int purity;
        unsigned int categories;

        size_t resolution_count;
        Resolution *resolutions;

        size_t aspect_ratio_count;
        Ratio *aspect_ratios;

        TopRange toplist_range;

        size_t tag_blacklist_count;
        const char **tag_blacklists;

        size_t user_blacklist_count;
        const char **user_blacklists;

        cJSON *json;
} Settings;

typedef struct Thumbs {
        const char *large;
        const char *original;
        const char *small;
} Thumbs;

typedef struct User {
        const char *user_name;
        const char *group;
        const char *avatar200px;
        const char *avatar128px;
        const char *avatar32px;
        const char *avatar20px;
} User;

typedef struct Wallpaper {
        const char *id;
        const char *url;
        const char *short_url;

        User uploader;

        size_t views;
        size_t favorites;

        const char *source;

        Purity purity;
        Category category;

        Resolution resolution;
        float ratio;

        size_t file_size;
        ImageType type;

        const char *created_at;

        size_t color_count;
        Color *colors;

        const char *path;

        Thumbs thumbs;

        size_t tag_count;
        Tag *tags;

        cJSON *json;
} Wallpaper;

typedef struct SearchResult {
        size_t wallpaper_count;
        Wallpaper *wallpapers;

        size_t current_page;
        size_t last_page;
        size_t per_page;
        size_t total;
        const char *seed;

        struct {
                union {
                        const char *query;

                        struct {
                                size_t id;
                                const char *tag;
                        };
                };

                enum { NORMAL_SEARCH, EXACT_TAG_SEARCH } type;
        };

        cJSON *json;
} SearchResult;

typedef struct Collection {
        unsigned int id;
        const char *label;
        unsigned int views;
        bool public;
        unsigned int count;
} Collection;

typedef struct Collections {
        size_t collection_count;
        Collection *collections;

        cJSON *json;
} Collections;

/**
 * @brief Get the wallpaper info.
 *
 * If apikey is set, uses it.
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
 * If apikey is set uses it.
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
WH_API bool whapi_get_tag_info(unsigned int id, Tag *tag);

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
                                                 unsigned int id,
                                                 unsigned int purity,
                                                 SearchResult *search_result);

/**
 * @brief Destroy the Tag returned.
 *
 * @param tag Pointer to tag
 */
WH_API void whapi_destroy_tag(Tag *tag);

/**
 * @brief Destroy the Settings returned.
 *
 * @param settings Pointer to settings
 */
WH_API void whapi_destroy_settings(Settings *settings);

/**
 * @brief Destroy the wallpaper returned.
 *
 * @param wallpaper Pointer to wallpaper
 */
WH_API void whapi_destroy_wallpaper(Wallpaper *wallpaper);

/**
 * @brief Destroy the search result returned.
 *
 * @param search_result Pointer to the search result
 */
WH_API void whapi_destroy_search_result(SearchResult *search_result);

/**
 * @brief Destroy the collections returned.
 *
 * @param collections Pointer to the collections
 */
WH_API void whapi_destroy_collections(Collections *collections);
