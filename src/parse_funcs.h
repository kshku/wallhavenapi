#pragma once

#include "cJSON.h"
#include "whapi/whparsed.h"

bool parse_wallpaper_info(cJSON *json, Wallpaper *wallpaper,
                          bool search_result);

bool parse_tag(cJSON *json, Tag *tag);

bool parse_search_result(cJSON *json, SearchResult *search_result,
                         bool exact_tag_search);
