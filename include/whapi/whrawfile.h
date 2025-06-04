#pragma once

#include "whapi/whcore.h"

/**
 * @brief Get the wallpaper info and write to file.
 *
 * If apikey is set, uses it.
 *
 * @param id The wallpaper id
 * @param file Path to file
 *
 * @return Returns false, on failure.
 */
WH_API bool whapi_get_wallpaper_info_raw_to_file(const char *id,
                                                 const char *file);
