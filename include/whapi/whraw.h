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
 * @param resbuffer Pointer to whStr, where the response is written
 *
 * @return Returns false on failure.
 */
WH_API bool whapi_search_raw(SearchParameters params, whStr *res);
