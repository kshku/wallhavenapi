#pragma once

#include "whapi/whcore.h"
#include "whapi/whstr.h"

/**
 * @brief Get the wallpaper info.
 *
 * If apikey is set, uses it.
 *
 * @param id The wallpaper id
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_wallpaper_info_raw(const char *id, whStr *res);

/**
 * @brief Search.
 *
 * If apikey is set uses it.
 *
 * @param params SearchParameters
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_search_raw(SearchParameters params, whStr *res);

/**
 * @brief Get the tag info.
 *
 * @param id Tag id
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_tag_info_raw(const char *id, whStr *res);

/**
 * @brief Get the user settings.
 *
 * Fails if apikey is not set.
 *
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_settings_raw(whStr *res);

/**
 * @brief List the collections.
 *
 * "Only collections that are public will be accessible to other users. When
 * authenticated, you are able to view all of your own collections"
 *
 * If user_name is NULL, then collection of the user is listed (apikey is must).
 *
 * @param user_name The username from which to list collections
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_collections_raw(const char *user_name, whStr *res);

/**
 * @brief Get the wallpapers from a collection.
 *
 * If apikey is set, it is used. To access private collections, need apikey.
 *
 * @param user_name Username (even if your own collection)
 * @param id The collection id
 * @param purity The purity filter
 * @param res Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_get_wallpapers_from_collection_raw(const char *user_name,
                                                     const char *id,
                                                     unsigned int purity,
                                                     whStr *res);
