#include "parse_funcs.h"

#include <stdlib.h>
#include <string.h>

#include "whcommon.h"

/**
 * @brief Key and pointer to string variable.
 */
typedef struct KeyStrVar {
        const char *key;
        const char **var;
} KeyStrVar;

/**
 * @brief Key and pointer to number variable.
 */
typedef struct KeyNumVar {
        const char *key;

        enum { UNSIGNED_INT, SIZE_T } type; /**< Type of the number */

        union {
                unsigned int *puint;
                size_t *psize_t;
        };
} KeyNumVar;

static bool parse_key_string_vars(cJSON *json, KeyStrVar *keystrvars,
                                  size_t len);

static bool parse_key_num_vars(cJSON *json, KeyNumVar *keynumvars, size_t len);

static bool parse_uploader(cJSON *json, User *uploader);

static bool parse_ratio(cJSON *json, float *ratio);

static bool parse_colors(cJSON *json, Color **colors, size_t *color_count);

static bool parse_purity(cJSON *json, Purity *purity);

static bool parse_category(cJSON *json, Category *category);

static bool parse_image_type(cJSON *json, ImageType *type);

static bool parse_tags(cJSON *json, Tag **tags, size_t *tag_count);

static bool parse_thumbs(cJSON *json, Thumbs *thumbs);

static bool parse_purities(cJSON *json, unsigned int *purity);

static bool parse_categories(cJSON *json, unsigned int *categories);

static bool parse_toprange(cJSON *json, TopRange *toprange);

static bool parse_resolutions_or_ratios(cJSON *json, Resolution **rrs,
                                        size_t *count, const char *name);

static bool parse_strings(cJSON *json, const char ***strings, size_t *count,
                          const char *name);

static bool parse_collection(cJSON *json, Collection *collection);

bool parse_wallpaper_info(cJSON *json, Wallpaper *wallpaper,
                          bool search_result) {
    cJSON *data =
        search_result ? json : cJSON_GetObjectItemCaseSensitive(json, "data");

    CHECKB_RETURN(cJSON_IsObject(data), false,
                  whapi_destroy_wallpaper(wallpaper));

    KeyStrVar keystrvars[] = {
        {        .key = "id",         .var = &wallpaper->id},
        {       .key = "url",        .var = &wallpaper->url},
        { .key = "short_url",  .var = &wallpaper->short_url},
        {    .key = "source",     .var = &wallpaper->source},
        {.key = "created_at", .var = &wallpaper->created_at},
        {      .key = "path",       .var = &wallpaper->path},
    };

    CHECKB_RETURN(
        parse_key_string_vars(data, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false, whapi_destroy_wallpaper(wallpaper));

    KeyNumVar keynumvars[] = {
        {      .key = "views",.psize_t = &wallpaper->views,.type = SIZE_T                                                                },
        {  .key = "favorites", .psize_t = &wallpaper->favorites, .type = SIZE_T},
        {.key = "dimension_x",
         .puint = &wallpaper->resolution.width,
         .type = UNSIGNED_INT                                                  },
        {.key = "dimension_y",
         .puint = &wallpaper->resolution.height,
         .type = UNSIGNED_INT                                                  },
        {  .key = "file_size", .psize_t = &wallpaper->file_size, .type = SIZE_T},
    };

    CHECKB_RETURN(
        parse_key_num_vars(data, keynumvars,
                           (sizeof(keynumvars) / sizeof(keynumvars[0]))),
        false, whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_purity(data, &wallpaper->purity), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_category(data, &wallpaper->category), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_image_type(data, &wallpaper->type), false,
                  whapi_destroy_wallpaper(wallpaper));

    if (!search_result)
        CHECKB_RETURN(parse_uploader(data, &wallpaper->uploader), false,
                      whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_ratio(data, &wallpaper->ratio), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(
        parse_colors(data, &wallpaper->colors, &wallpaper->color_count), false,
        whapi_destroy_wallpaper(wallpaper));

    if (!search_result)
        CHECKB_RETURN(parse_tags(data, &wallpaper->tags, &wallpaper->tag_count),
                      false, whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_thumbs(data, &wallpaper->thumbs), false,
                  whapi_destroy_wallpaper(wallpaper));

    return true;
}

bool parse_tag(cJSON *json, Tag *tag, bool no_data) {
    cJSON *data =
        no_data ? json : cJSON_GetObjectItemCaseSensitive(json, "data");

    CHECKB_RETURN(cJSON_IsObject(data), false, whapi_destroy_tag(tag));

    KeyStrVar keystrvars[] = {
        {      .key = "name",       .var = &tag->name},
        {     .key = "alias",      .var = &tag->alias},
        {  .key = "category",   .var = &tag->category},
        {.key = "created_at", .var = &tag->created_at},
    };
    KeyNumVar keynumvars[] = {
        {         .key = "id",          .psize_t = &tag->id, .type = SIZE_T},
        {.key = "category_id", .psize_t = &tag->category_id, .type = SIZE_T},
    };

    CHECKB_RETURN(parse_purity(data, &tag->purity), false,
                  whapi_destroy_tag(tag));
    CHECKB_RETURN(
        parse_key_string_vars(data, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false, whapi_destroy_tag(tag));
    CHECKB_RETURN(
        parse_key_num_vars(data, keynumvars,
                           (sizeof(keynumvars) / sizeof(keynumvars[0]))),
        false, whapi_destroy_tag(tag));

    return true;
}

bool parse_search_result(cJSON *json, SearchResult *search_result,
                         bool exact_tag_search, bool collection) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "data");

    CHECKB_RETURN(cJSON_IsArray(data), false,
                  whapi_destroy_search_result(search_result));

    search_result->wallpaper_count = cJSON_GetArraySize(data);

    search_result->wallpapers =
        (Wallpaper *)calloc(search_result->wallpaper_count, sizeof(Wallpaper));
    CHECKP_RETURN(search_result->wallpapers, false,
                  whapi_destroy_search_result(search_result));

    size_t i = 0;
    cJSON *wallpaper;
    cJSON_ArrayForEach(wallpaper, data) {
        CHECKB_RETURN(cJSON_IsObject(wallpaper), false,
                      whapi_destroy_search_result(search_result));
        CHECKB_RETURN(parse_wallpaper_info(wallpaper,
                                           &search_result->wallpapers[i], true),
                      false, whapi_destroy_search_result(search_result));
        ++i;
    }

    cJSON *meta = cJSON_GetObjectItemCaseSensitive(json, "meta");
    CHECKB_RETURN(cJSON_IsObject(meta), false,
                  whapi_destroy_search_result(search_result));

    // sometimes the per_page is number and sometimes it is string
    KeyNumVar keynumvars[] = {
        {.key = "current_page",
         .psize_t = &search_result->current_page,
         .type = SIZE_T                                                         },
        {   .key = "last_page",
         .psize_t = &search_result->last_page,
         .type = SIZE_T                                                         },
        {       .key = "total", .psize_t = &search_result->total, .type = SIZE_T},
    };

    CHECKB_RETURN(
        parse_key_num_vars(meta, keynumvars,
                           (sizeof(keynumvars) / sizeof(keynumvars[0]))),
        false, whapi_destroy_search_result(search_result));

    cJSON *per_page = cJSON_GetObjectItemCaseSensitive(meta, "per_page");
    if (cJSON_IsString(per_page))
        sscanf(per_page->valuestring, "%zu", &search_result->per_page);
    else if (cJSON_IsNumber(per_page))
        search_result->per_page = (size_t)per_page->valuedouble;
    else
        CHECKB_RETURN(false, false, whapi_destroy_search_result(search_result));

    if (!collection) {
        KeyStrVar seed = {.key = "seed", .var = &search_result->seed};

        CHECKB_RETURN(parse_key_string_vars(meta, &seed, 1), false,
                      whapi_destroy_search_result(search_result));

        if (exact_tag_search) {
            search_result->type = EXACT_TAG_SEARCH;
            cJSON *query = cJSON_GetObjectItemCaseSensitive(meta, "query");
            CHECKB_RETURN(cJSON_IsObject(query), false,
                          whapi_destroy_search_result(search_result));

            KeyNumVar id = {
                .key = "id", .psize_t = &search_result->id, .type = SIZE_T};
            KeyStrVar tag = {.key = "tag", .var = &search_result->tag};
            CHECKB_RETURN(parse_key_num_vars(query, &id, 1), false,
                          whapi_destroy_search_result(search_result));

            CHECKB_RETURN(parse_key_string_vars(query, &tag, 1), false,
                          whapi_destroy_search_result(search_result));
        } else {
            search_result->type = NORMAL_SEARCH;

            KeyStrVar query = {.key = "query", .var = &search_result->query};
            CHECKB_RETURN(parse_key_string_vars(meta, &query, 1), false,
                          whapi_destroy_search_result(search_result));
        }
    }

    return true;
}

bool parse_settings(cJSON *json, Settings *settings) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "data");
    CHECKB_RETURN(cJSON_IsObject(data), false,
                  whapi_destroy_settings(settings));

    const char *buf;
    KeyStrVar keystrvars[] = {
        {.key = "thumb_size", .var = &settings->thumb_size},
        {  .key = "per_page",                  .var = &buf},
    };

    CHECKB_RETURN(
        parse_key_string_vars(data, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false, whapi_destroy_settings(settings));

    sscanf(buf, "%zu", &settings->per_page);

    CHECKB_RETURN(parse_purities(data, &settings->purity), false,
                  whapi_destroy_settings(settings));

    CHECKB_RETURN(parse_categories(data, &settings->categories), false,
                  whapi_destroy_settings(settings));

    CHECKB_RETURN(parse_toprange(data, &settings->toplist_range), false,
                  whapi_destroy_settings(settings));

    CHECKB_RETURN(
        parse_resolutions_or_ratios(data, &settings->resolutions,
                                    &settings->resolution_count, "resolutions"),
        false, whapi_destroy_settings(settings));

    CHECKB_RETURN(parse_resolutions_or_ratios(data, &settings->aspect_ratios,
                                              &settings->aspect_ratio_count,
                                              "aspect_ratios"),
                  false, whapi_destroy_settings(settings));

    CHECKB_RETURN(
        parse_strings(data, &settings->tag_blacklist,
                      &settings->tag_blacklist_count, "tag_blacklist"),
        false, whapi_destroy_settings(settings));

    CHECKB_RETURN(
        parse_strings(data, &settings->user_blacklist,
                      &settings->user_blacklist_count, "user_blacklist"),
        false, whapi_destroy_settings(settings));

    return true;
}

bool parse_collections(cJSON *json, Collections *collections) {
    cJSON *data = cJSON_GetObjectItem(json, "data");
    CHECKB_RETURN(cJSON_IsArray(data), false,
                  whapi_destroy_collections(collections));

    collections->collection_count = cJSON_GetArraySize(data);

    collections->collections =
        (Collection *)calloc(collections->collection_count, sizeof(Collection));
    CHECKP_RETURN(collections->collections, false,
                  whapi_destroy_collections(collections));

    size_t i = 0;
    cJSON *collection;
    cJSON_ArrayForEach(collection, data) {
        CHECKB_RETURN(cJSON_IsObject(collection), false,
                      whapi_destroy_collections(collections));
        CHECKB_RETURN(
            parse_collection(collection, &collections->collections[i]), false,
            whapi_destroy_collections(collections));
        ++i;
    }

    return true;
}

static bool parse_key_string_vars(cJSON *json, KeyStrVar *keystrvars,
                                  size_t len) {
    for (size_t i = 0; i < len; ++i) {
        cJSON *item = cJSON_GetObjectItemCaseSensitive(json, keystrvars[i].key);

        CHECKB_RETURN((cJSON_IsString(item) && (item->valuestring != NULL))
                          || cJSON_IsNull(item),
                      false);

        *keystrvars[i].var = cJSON_IsString(item) ? item->valuestring : NULL;
    }

    return true;
}

static bool parse_key_num_vars(cJSON *json, KeyNumVar *keynumvars, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        cJSON *item = cJSON_GetObjectItemCaseSensitive(json, keynumvars[i].key);

        CHECKB_RETURN(cJSON_IsNumber(item), false);

        if (keynumvars[i].type == UNSIGNED_INT)
            *keynumvars[i].puint = (unsigned int)item->valueint;
        else *keynumvars[i].psize_t = (size_t)item->valuedouble;
    }

    return true;
}

static bool parse_uploader(cJSON *json, User *uploader) {
    cJSON *up = cJSON_GetObjectItemCaseSensitive(json, "uploader");

    CHECKB_RETURN(cJSON_IsObject(up), false);

    KeyStrVar keystrvars[] = {
        {.key = "username", .var = &uploader->user_name},
        {   .key = "group",     .var = &uploader->group},
    };

    KeyStrVar keystrvars2[] = {
        {.key = "200px", .var = &uploader->avatar200px},
        {.key = "128px", .var = &uploader->avatar128px},
        { .key = "32px",  .var = &uploader->avatar32px},
        { .key = "20px",  .var = &uploader->avatar20px},
    };

    CHECKB_RETURN(
        parse_key_string_vars(up, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false);

    cJSON *avatar = cJSON_GetObjectItemCaseSensitive(up, "avatar");

    CHECKB_RETURN(cJSON_IsObject(avatar), false);

    CHECKB_RETURN(
        parse_key_string_vars(avatar, keystrvars2,
                              (sizeof(keystrvars2) / sizeof(keystrvars2[0]))),
        false);

    return true;
}

static bool parse_ratio(cJSON *json, float *ratio) {
    cJSON *sratio = cJSON_GetObjectItemCaseSensitive(json, "ratio");

    CHECKB_RETURN(cJSON_IsString(sratio) && sratio->valuestring != NULL, false);

    sscanf(sratio->valuestring, "%f", ratio);

    return true;
}

static bool parse_colors(cJSON *json, Color **colors, size_t *color_count) {
    cJSON *clrs = cJSON_GetObjectItemCaseSensitive(json, "colors");
    CHECKB_RETURN(cJSON_IsArray(clrs), false);
    *color_count = cJSON_GetArraySize(clrs);

    *colors = (Color *)malloc(sizeof(Color) * (*color_count));
    CHECKP_RETURN(*colors, false);
    cJSON *color;
    size_t i = 0;
    cJSON_ArrayForEach(color, clrs) {
        CHECKB_RETURN(cJSON_IsString(color) && color->valuestring != NULL,
                      false);
        sscanf(color->valuestring, "#%06x", (unsigned int *)(&((*colors)[i])));
        ++i;
    }

    return true;
}

static bool parse_purity(cJSON *json, Purity *purity) {
    cJSON *pty = cJSON_GetObjectItemCaseSensitive(json, "purity");

    CHECKB_RETURN(cJSON_IsString(pty) && pty->valuestring != NULL, false);

    if (!strncmp(pty->valuestring, "sfw", 3)) *purity = PURITY_SFW;
    else if (!strncmp(pty->valuestring, "nsfw", 4)) *purity = PURITY_NSFW;
    else *purity = PURITY_SKETCHY;

    return true;
}

static bool parse_category(cJSON *json, Category *category) {
    cJSON *ctg = cJSON_GetObjectItemCaseSensitive(json, "category");

    CHECKB_RETURN(cJSON_IsString(ctg) && ctg->valuestring != NULL, false);

    if (!strncmp(ctg->valuestring, "anime", 5)) *category = CATEGORY_ANIME;
    else if (!strncmp(ctg->valuestring, "people", 6))
        *category = CATEGORY_PEOPLE;
    else *category = CATEGORY_GENERAL;

    return true;
}

static bool parse_image_type(cJSON *json, ImageType *type) {
    cJSON *image_type = cJSON_GetObjectItemCaseSensitive(json, "file_type");

    CHECKB_RETURN(cJSON_IsString(image_type) && image_type->valuestring != NULL,
                  false);

    if (!strncmp(image_type->valuestring, "image/png", 9))
        *type = IMAGE_TYPE_PNG;
    else *type = IMAGE_TYPE_JPG;

    return true;
}

static bool parse_tags(cJSON *json, Tag **tags, size_t *tag_count) {
    cJSON *tgs = cJSON_GetObjectItemCaseSensitive(json, "tags");
    CHECKB_RETURN(cJSON_IsArray(tgs), false);
    *tag_count = cJSON_GetArraySize(tgs);

    *tags = (Tag *)calloc((*tag_count), sizeof(Tag));
    CHECKP_RETURN(*tags, false);
    size_t i = 0;
    cJSON *tag;
    cJSON_ArrayForEach(tag, tgs) {
        CHECKB_RETURN(cJSON_IsObject(tag), false);
        CHECKB_RETURN(parse_tag(tag, &((*tags)[i]), true), false);
        ++i;
    }

    return true;
}

static bool parse_thumbs(cJSON *json, Thumbs *thumbs) {
    cJSON *tms = cJSON_GetObjectItemCaseSensitive(json, "thumbs");
    CHECKB_RETURN(cJSON_IsObject(tms), false);

    KeyStrVar keystrvars[] = {
        {   .key = "large",    .var = &thumbs->large},
        {.key = "original", .var = &thumbs->original},
        {   .key = "small",    .var = &thumbs->small},
    };

    CHECKB_RETURN(
        parse_key_string_vars(tms, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false);

    return true;
}

static bool parse_purities(cJSON *json, unsigned int *purity) {
    cJSON *pty = cJSON_GetObjectItemCaseSensitive(json, "purity");
    CHECKB_RETURN(cJSON_IsArray(pty), false);

    *purity = PURITY_NONE;

    cJSON *p;
    cJSON_ArrayForEach(p, pty) {
        CHECKB_RETURN(cJSON_IsString(p), false);

        if ((!((*purity) & PURITY_SFW)) && (!strncmp(p->valuestring, "sfw", 3)))
            *purity |= PURITY_SFW;
        else if ((!((*purity) & PURITY_NSFW))
                 && (!strncmp(p->valuestring, "nsfw", 4)))
            *purity |= PURITY_NSFW;
        else *purity |= PURITY_SKETCHY;
    }

    return true;
}

static bool parse_categories(cJSON *json, unsigned int *categories) {
    cJSON *ctg = cJSON_GetObjectItemCaseSensitive(json, "categories");
    CHECKB_RETURN(cJSON_IsArray(ctg), false);

    *categories = CATEGORY_NONE;

    cJSON *c;
    cJSON_ArrayForEach(c, ctg) {
        CHECKB_RETURN(cJSON_IsString(c), false);

        if ((!((*categories) & CATEGORY_ANIME))
            && (!strncmp(c->valuestring, "anime", 5)))
            *categories |= CATEGORY_ANIME;
        else if ((!((*categories) & CATEGORY_PEOPLE))
                 && (!strncmp(c->valuestring, "people", 6)))
            *categories |= CATEGORY_PEOPLE;
        else *categories |= CATEGORY_GENERAL;
    }

    return true;
}

static bool parse_toprange(cJSON *json, TopRange *toprange) {
    cJSON *tr = cJSON_GetObjectItem(json, "toplist_range");
    CHECKB_RETURN(cJSON_IsString(tr), false);

    *toprange = TOPRANGE_NONE;

    switch (tr->valuestring[0]) {
        case '1': {
            switch (tr->valuestring[1]) {
                case 'd':
                    *toprange = TOPRANGE_1D;
                    break;
                case 'w':
                    *toprange = TOPRANGE_1W;
                    break;
                case 'M':
                    *toprange = TOPRANGE_1M;
                    break;
                case 'y':
                    *toprange = TOPRANGE_1Y;
                    break;
                default:
                    return false;
            }
        } break;
        case '3': {
            switch (tr->valuestring[1]) {
                case 'd':
                    *toprange = TOPRANGE_3D;
                    break;
                case 'M':
                    *toprange = TOPRANGE_3M;
                    break;
                default:
                    return false;
            }
        } break;
        case '6':
            *toprange = TOPRANGE_6M;
            break;
        default:
            return false;
    }

    return true;
}

static bool parse_resolutions_or_ratios(cJSON *json, Resolution **rrs,
                                        size_t *count, const char *name) {
    cJSON *items = cJSON_GetObjectItem(json, name);
    CHECKB_RETURN(cJSON_IsArray(items), false);
    *count = cJSON_GetArraySize(items);

    *rrs = (Resolution *)calloc((*count), sizeof(Resolution));

    size_t i = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, items) {
        CHECKB_RETURN(cJSON_IsString(item) && item->valuestring != NULL, false);
        sscanf(item->valuestring, "%ux%u", &((*rrs)[i]).width,
               &((*rrs)[i]).height);
        ++i;
    }

    return true;
}

static bool parse_strings(cJSON *json, const char ***strings, size_t *count,
                          const char *name) {
    cJSON *items = cJSON_GetObjectItem(json, name);
    CHECKB_RETURN(cJSON_IsArray(items), false);
    *count = cJSON_GetArraySize(items);

    *strings = (const char **)malloc((*count) * sizeof(const char *));

    size_t i = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, items) {
        CHECKB_RETURN(cJSON_IsString(item), false);
        (*strings)[i] = item->valuestring;
        ++i;
    }

    return true;
}

static bool parse_collection(cJSON *json, Collection *collection) {
    CHECKB_RETURN(cJSON_IsObject(json), false);

    unsigned int public;
    KeyNumVar keynumvars[] = {
        {    .key = "id",    .psize_t = &collection->id,       .type = SIZE_T},
        { .key = "views", .psize_t = &collection->views,       .type = SIZE_T},
        {.key = "public",              .puint = &public, .type = UNSIGNED_INT},
        { .key = "count", .psize_t = &collection->count,       .type = SIZE_T},
    };

    CHECKB_RETURN(
        parse_key_num_vars(json, keynumvars,
                           (sizeof(keynumvars) / sizeof(keynumvars[0]))),
        false);

    collection->public = (bool)public;

    KeyStrVar label = {.key = "label", .var = &collection->label};
    CHECKB_RETURN(parse_key_string_vars(json, &label, 1), false);

    return true;
}
