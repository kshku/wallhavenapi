#pragma once

#include "whapi/whcore.h"

/**
 * @brief Get the wallpaper info and write to file.
 *
 * Uses apikey if set.
 *
 * @param id The wallpaper id
 * @param file Path to file
 *
 * @return Returns false, on failure.
 */
WH_API bool whapi_get_wallpaper_info_raw_to_file(const char *id,
                                                 const char *file);

/**
 * @brief Search.
 *
 * Uses apikey if set.
 *
 * @param params SearchParameters
 * @param file Path to file
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_search_raw_to_file(SearchParameters params, const char *file);

/**
 * @brief Get the tag info.
 *
 * @param id Tag id
 * @param file Path to file
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_tag_info_raw_to_file(size_t id, const char *file);

/**
 * @brief Get the user settings.
 *
 * Fails if apikey is not set.
 *
 * @param file Path to file
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_settings_raw_to_file(const char *file);

/**
 * @brief List the collections.
 *
 * "Only collections that are public will be accessible to other users. When
 * authenticated, you are able to view all of your own collections"
 *
 * If user_name is NULL, then collection of the user is listed (apikey is must).
 *
 * @param user_name The username from which to list collections
 * @param file Path to file
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_collections_raw_to_file(const char *user_name,
                                              const char *file);

/**
 * @brief Get the wallpapers from a collection.
 *
 * If apikey is set, it is used. To access private collections, need apikey.
 *
 * @param user_name Username (even if your own collection)
 * @param id The collection id
 * @param purity The purity filter
 * @param file Path to file
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_wallpapers_from_collection_raw_to_file(
    const char *user_name, size_t id, unsigned int purity, const char *file);
