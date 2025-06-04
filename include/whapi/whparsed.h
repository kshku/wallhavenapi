#pragma once

#include "whapi/whcore.h"

/**
 * @brief Get the wallpaper info.
 *
 * If apikey is set, uses it.
 *
 * @param id The wallpaper id
 * @todo
 *
 * @return Returns false, on failure.
 */
WH_API bool whapi_get_wallpaper_info(const char *id, void *ptr);
