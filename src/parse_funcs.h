#pragma once

#include "cJSON.h"
#include "whapi/whparsed.h"

bool parse_wallpaper_info(cJSON *json, Wallpaper *wallpaper,
                          bool search_result);

bool parse_tag(cJSON *json, Tag *tag, bool no_data);

bool parse_search_result(cJSON *json, SearchResult *search_result,
                         bool exact_tag_search, bool collection);

bool parse_settings(cJSON *json, Settings *settings);

bool parse_collections(cJSON *json, Collections *collections);
