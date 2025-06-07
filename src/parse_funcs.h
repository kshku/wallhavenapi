#pragma once

#include <string.h>

#include "cJSON.h"
#include "whapi/whparsed.h"
#include "whcommon.h"

typedef struct KeyStrVar {
        const char *key;
        const char **var;
} KeyStrVar;

typedef struct KeyNumVar {
        const char *key;

        enum { UNSIGNED_INT, SIZE_T } type;

        union {
                unsigned int *puint;
                size_t *psize_t;
        };
} KeyNumVar;

static bool parse_key_string_vars(cJSON *json, KeyStrVar *keystrvars,
                                  size_t len) {
    for (size_t i = 0; i < len; ++i) {
        cJSON *item = cJSON_GetObjectItemCaseSensitive(json, keystrvars[i].key);

        CHECKB_RETURN(cJSON_IsString(item) && item->valuestring != NULL, false);

        *keystrvars[i].var = item->valuestring;
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
        parse_key_string_vars(json, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false);

    cJSON *avatar = cJSON_GetObjectItemCaseSensitive(json, "avatar");
    CHECKB_RETURN(cJSON_IsObject(avatar), false);

    CHECKB_RETURN(
        parse_key_string_vars(avatar, keystrvars2,
                              (sizeof(keystrvars2) / sizeof(keystrvars2[0]))),
        false);

    return true;
}

static bool parse_ratio(cJSON *json, Wallpaper *wallpaper) {
    cJSON *sratio = cJSON_GetObjectItemCaseSensitive(json, "ratio");

    CHECKB_RETURN(cJSON_IsString(sratio) && sratio->valuestring != NULL, false);

    sscanf(sratio->valuestring, "%f", &wallpaper->ratio);

    return true;
}

static bool parse_colors(cJSON *json, Wallpaper *wallpaper) {
    cJSON *clrs = cJSON_GetObjectItemCaseSensitive(json, "colors");
    CHECKB_RETURN(cJSON_IsArray(clrs), false);
    wallpaper->color_count = cJSON_GetArraySize(clrs);

    wallpaper->colors = (Color *)malloc(sizeof(Color) * wallpaper->color_count);
    CHECKP_RETURN(wallpaper->colors, false);
    cJSON *color;
    size_t i = 0;
    cJSON_ArrayForEach(color, clrs) {
        CHECKB_RETURN(cJSON_IsString(color) && color->valuestring != NULL,
                      false);
        sscanf(color->valuestring, "#%06x",
               (unsigned int *)(&wallpaper->colors[i]));
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

static bool parse_tag(cJSON *json, Tag *tag) {
    *tag = (Tag){0};
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
    CHECKB_RETURN(parse_purity(json, &tag->purity), false);
    CHECKB_RETURN(
        parse_key_string_vars(json, keystrvars,
                              (sizeof(keystrvars) / sizeof(keystrvars[0]))),
        false);
    CHECKB_RETURN(
        parse_key_num_vars(json, keynumvars,
                           (sizeof(keynumvars) / sizeof(keynumvars[0]))),
        false);

    return true;
}

static bool parse_tags(cJSON *json, Tag **tags, size_t *tag_count) {
    cJSON *tgs = cJSON_GetObjectItemCaseSensitive(json, "tags");
    CHECKB_RETURN(cJSON_IsArray(tgs), false);
    *tag_count = cJSON_GetArraySize(tgs);

    *tags = (Tag *)malloc(sizeof(Tag) * (*tag_count));
    CHECKP_RETURN(*tags, false);
    size_t i = 0;
    cJSON *tag;
    cJSON_ArrayForEach(tag, tgs) {
        CHECKB_RETURN(cJSON_IsObject(tag), false);
        CHECKB_RETURN(parse_tag(tag, &((*tags)[i])), false);
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

static bool parse_wallpaper_info(Wallpaper *wallpaper, whStr *res) {
    *wallpaper = (Wallpaper){0};
    wallpaper->json = cJSON_ParseWithLength(res->str, res->len);

    CHECKP_RETURN(wallpaper->json, false,
                  whapi.error_code = WALLHAVEN_PARSING_FAILED,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    cJSON *data = cJSON_GetObjectItemCaseSensitive(wallpaper->json, "data");
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

    cJSON *uploader = cJSON_GetObjectItemCaseSensitive(data, "uploader");
    CHECKB_RETURN(cJSON_IsObject(uploader), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_uploader(uploader, &wallpaper->uploader), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_ratio(data, wallpaper), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_colors(data, wallpaper), false,
                  whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_tags(data, &wallpaper->tags, &wallpaper->tag_count),
                  false, whapi_destroy_wallpaper(wallpaper));

    CHECKB_RETURN(parse_thumbs(data, &wallpaper->thumbs), false,
                  whapi_destroy_wallpaper(wallpaper));

    return true;
}
