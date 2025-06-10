#include "whcommon.h"

// Global state
whAPI whapi = {0};

static bool reset_url(void);

static bool append_query(const char *key, const char *value);

static bool concat_and_set_path(const char *p, const char *q);

static void set_response_code_error(unsigned int response_code);

bool perform_call(void) {
#ifdef WH_DEBUG
    char *path, buffer[4096];
    path = buffer;
    CHECK_RETURN(whapi.error_code = curl_url_get(whapi.url, CURLUPART_URL,
                                                 &path, CURLU_URLDECODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);
    printf("URL: %s\n", path);
#endif

    CHECK_RETURN(whapi.error_code = curl_easy_perform(whapi.curl), false,
                 whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    long response_code;

    CHECK_RETURN(whapi.error_code = curl_easy_getinfo(
                     whapi.curl, CURLINFO_RESPONSE_CODE, &response_code),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_CURL);

    set_response_code_error(response_code);
    whapi.retry = whapi.response_code_handler(response_code);

    return true;
}

static bool format_and_append_q(const Query *q) {
    whStr query = whstr_create();

    if (q->id) {
        if ((q->like || q->tags || q->type != IMAGE_TYPE_NONE
             || q->user_name)) {
            whstr_destroy(&query);
            whapi.error_code = WALLHAVEN_CANNOT_COMBINE_ID,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            return false;
        }

        CHECKB_RETURN(whstr_setf(&query, "id:%u", q->id), false,
                      whstr_destroy(&query));

    } else {
        if (q->tags)
            CHECKB_RETURN(whstr_set(&query, q->tags), false,
                          whstr_destroy(&query));

        if (q->user_name) {
            CHECKB_RETURN(whstr_appendf(&query, " @%s", q->user_name), false,
                          whstr_destroy(&query));
        }

        if (q->type != IMAGE_TYPE_NONE) {
            CHECKB_RETURN(
                whstr_appendf(&query, " type:%s",
                              q->type == IMAGE_TYPE_PNG ? "png" : "jpg"),
                false, whstr_destroy(&query));
        }

        if (q->like) {
            CHECKB_RETURN(whstr_appendf(&query, " like:%s", q->like), false,
                          whstr_destroy(&query));
        }
    }

    if (query.str)
        CHECKB_RETURN(append_query("q", query.str), false,
                      whstr_destroy(&query));

    whstr_destroy(&query);

    return true;
}

static bool format_and_append_categories(unsigned int categories) {
    if (categories == 0) return true;

    // char cat[4] = "000";
    // if (categories & CATEGORY_GENERAL) cat[0] = '1';
    // if (categories & CATEGORY_ANIME) cat[1] = '1';
    // if (categories & CATEGORY_PEOPLE) cat[2] = '1';

    char cat[4] = {0};
    for (int i = 0; i < 3; ++i, categories >>= 1)
        cat[i] = (categories & 1) + '0';

    CHECKB_RETURN(append_query("categories", cat), false, (void)0);

    return true;
}

static bool format_and_append_purity(unsigned int purity) {
    if (purity == 0) return true;

    if (purity & PURITY_NSFW)
        CHECKP_RETURN(whapi.apikey.str, false,
                      whapi.error_code = WALLHAVEN_NO_API_KEY,
                      whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    char pur[4] = {0};
    for (int i = 0; i < 3; ++i, purity >>= 1) pur[i] = (purity & 1) + '0';

    CHECKB_RETURN(append_query("purity", pur), false, (void)0);

    return true;
}

static bool format_and_append_sorting(Sorting sorting) {
    if (sorting == SORTING_NONE) return true;

    char *sor[] = {
        [SORTING_DATE_ADDED] = "date_added", [SORTING_RELEVANCE] = "relevance",
        [SORTING_RANDOM] = "random",         [SORTING_VIEWS] = "views",
        [SORTING_FAVORITES] = "favorites",   [SORTING_TOPLIST] = "toplist"};

    CHECKB_RETURN(append_query("sorting", sor[sorting]), false, (void)0);

    return true;
}

static bool format_and_append_order(Order order) {
    if (order == ORDER_NONE) return true;

    char *ord[] = {[ORDER_ASCENDING] = "asc", [ORDER_DESCENDING] = "desc"};

    CHECKB_RETURN(append_query("order", ord[order]), false, (void)0);

    return true;
}

static bool format_and_append_toprange(TopRange toprange, Sorting sorting) {
    if (toprange == TOPRANGE_NONE) return true;

    if (sorting != SORTING_TOPLIST) {
        whapi.error_code = WALLHAVEN_SORTING_MUST_BE_TOPLIST,
        whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
        return false;
    }

    char *top[] = {
        [TOPRANGE_1D] = "1d", [TOPRANGE_3D] = "3d", [TOPRANGE_1W] = "1w",
        [TOPRANGE_1M] = "1M", [TOPRANGE_3M] = "3M", [TOPRANGE_6M] = "6M",
        [TOPRANGE_1Y] = "1y"};

    CHECKB_RETURN(append_query("topRange", top[toprange]), false, (void)0);

    return true;
}

static bool format_and_append_atleast_resolution(Resolution res) {
    if (res.width == 0 && res.height == 0) return true;

    char buffer[20];

    snprintf(buffer, 20, "%ux%u", res.width, res.height);

    CHECKB_RETURN(append_query("atleast", buffer), false, (void)0);

    return true;
}

static bool format_and_append_exact_resolution(const Resolution *res,
                                               size_t n) {
    if (n == 0) return true;

    whStr resolutions = whstr_create();
    char buffer[20] = {0};
    size_t len = snprintf(buffer, 20, "%ux%u", res[0].width, res[0].height);
    CHECKB_RETURN(whstr_setn(&resolutions, buffer, len), false,
                  whstr_destroy(&resolutions));

    for (size_t i = 1; i < n; ++i) {
        len = snprintf(buffer, 20, ",%ux%u", res[i].width, res[i].height);
        CHECKB_RETURN(whstr_appendn(&resolutions, buffer, len), false,
                      whstr_destroy(&resolutions));
    }

    CHECKB_RETURN(append_query("resolutions", resolutions.str), false,
                  whstr_destroy(&resolutions));

    whstr_destroy(&resolutions);

    return true;
}

static bool format_and_append_ratios(const Ratio *ratios, size_t n) {
    if (n == 0) return true;

    whStr ra = whstr_create();
    char buffer[20] = {0};
    size_t len =
        snprintf(buffer, 20, "%ux%u", ratios[0].width, ratios[0].height);
    CHECKB_RETURN(whstr_setn(&ra, buffer, len), false, whstr_destroy(&ra));

    for (size_t i = 1; i < n; ++i) {
        len = snprintf(buffer, 20, ",%ux%u", ratios[i].width, ratios[i].height);
        CHECKB_RETURN(whstr_appendn(&ra, buffer, len), false,
                      whstr_destroy(&ra));
    }

    CHECKB_RETURN(append_query("ratios", ra.str), false, whstr_destroy(&ra));

    whstr_destroy(&ra);

    return true;
}

static bool format_and_append_colors(const Color *colors, size_t n) {
    if (n == 0) return true;

    whStr col = whstr_create();
    char buffer[10] = {0};
    size_t len = snprintf(buffer, 10, "%06x", colors[0]);
    CHECKB_RETURN(whstr_setn(&col, buffer, len), false, whstr_destroy(&col));

    for (size_t i = 1; i < n; ++i) {
        CHECKB_RETURN(whstr_appendf(&col, ",%06x", colors[i]), false,
                      whstr_destroy(&col));
    }

    CHECKB_RETURN(append_query("colors", col.str), false, whstr_destroy(&col));

    whstr_destroy(&col);

    return true;
}

static bool format_and_append_page(size_t page) {
    if (page == 0) return true;

    char buffer[25];
    snprintf(buffer, 25, "%zu", page);

    CHECKB_RETURN(append_query("page", buffer), false, (void)0);

    return true;
}

static bool format_and_append_seed(const char seed[7]) {
    if (seed[0] == 0) return true;

    for (int i = 0; seed[i]; ++i) {
        if ((seed[i] < 'a' || seed[i] > 'z') && (seed[i] < 'A' || seed[i] > 'Z')
            && (seed[i] < '0' || seed[i] > '9')) {
            whapi.error_code = WALLHAVEN_INVALID_SEED,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            return false;
        }
    }

    CHECKB_RETURN(append_query("seed", seed), false, (void)0);

    return true;
}

static bool format_and_append_search_parameters(
    const SearchParameters *params) {
    CHECKB_RETURN(format_and_append_q(&params->q), false, (void)0);

    CHECKB_RETURN(format_and_append_categories(params->categories), false,
                  (void)0);

    CHECKB_RETURN(format_and_append_purity(params->purity), false, (void)0);

    CHECKB_RETURN(format_and_append_sorting(params->sorting), false, (void)0);

    CHECKB_RETURN(format_and_append_order(params->order), false, (void)0);

    CHECKB_RETURN(format_and_append_toprange(params->toprange, params->sorting),
                  false, (void)0);

    if (!params->exact_resolution_count) {
        CHECKB_RETURN(
            format_and_append_atleast_resolution(params->atleast_resolution),
            false, (void)0);
    } else {
        CHECKB_RETURN(
            format_and_append_exact_resolution(params->exact_resolutions,
                                               params->exact_resolution_count),
            false, (void)0);
    }

    CHECKB_RETURN(format_and_append_ratios(params->ratios, params->ratio_count),
                  false, (void)0);

    CHECKB_RETURN(format_and_append_colors(params->colors, params->color_count),
                  false, (void)0);

    CHECKB_RETURN(format_and_append_page(params->page), false, (void)0);

    CHECKB_RETURN(format_and_append_seed(params->seed), false, (void)0);

    return true;
}

bool setup_wallpaper_info_url(const char *id) {
    CHECKP_RETURN(id, false, whapi.error_code = WALLHAVEN_NULL_ID,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(reset_url(), false, (void)0);

    CHECKB_RETURN(concat_and_set_path(WALLPAPER_INFO_PATH, id), false, (void)0);

    if (whapi.apikey.str)
        CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false, (void)0);

    return true;
}

bool setup_search_url(const SearchParameters *params) {
    CHECKB_RETURN(reset_url(), false, (void)0);

    CHECK_RETURN(whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                                 SEARCH_PATH, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    if (whapi.apikey.str)
        CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false, (void)0);

    CHECKB_RETURN(format_and_append_search_parameters(params), false, (void)0);

    return true;
}

bool setup_tag_info_url(size_t id) {
    CHECKB_RETURN(reset_url(), false, (void)0);

    whStr temp = whstr_create();

    CHECKB_RETURN(whstr_setf(&temp, "%u", id), false, whstr_destroy(&temp));

    CHECKB_RETURN(concat_and_set_path(TAG_INFO_PATH, temp.str), false,
                  whstr_destroy(&temp));

    whstr_destroy(&temp);

    return true;
}

bool setup_settings_url(void) {
    CHECKP_RETURN(whapi.apikey.str, false,
                  whapi.error_code = WALLHAVEN_NO_API_KEY,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(reset_url(), false, (void)0);

    CHECK_RETURN(
        whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                        USER_SETTINGS_PATH, CURLU_URLENCODE),
        false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false, (void)0);

    return true;
}

bool setup_collections_url(const char *user_name) {
    CHECKB_RETURN(whapi.apikey.str || user_name, false,
                  whapi.error_code = WALLHAVEN_NO_API_KEY,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(reset_url(), false, (void)0);

    if (user_name) {
        CHECKB_RETURN(concat_and_set_path(COLLECTIONS_PATH "/", user_name),
                      false, (void)0);
        if (whapi.apikey.str)
            CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false,
                          (void)0);
    } else {
        CHECK_RETURN(
            whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                            COLLECTIONS_PATH, CURLU_URLENCODE),
            false, whapi.error_code_type = ERROR_CODE_TYPE_URL);
        CHECKB_RETURN(append_query("apikey", whapi.apikey.str), false, (void)0);
    }

    return true;
}

bool setup_wallpaper_from_collection_url(const char *user_name, size_t id,
                                         unsigned int purity) {
    CHECKP_RETURN(user_name, false, whapi.error_code = WALLHAVEN_NULL_USER_NAME,
                  whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN);

    CHECKB_RETURN(reset_url(), false, (void)0);

    whStr unid = whstr_create();

    CHECKB_RETURN(whstr_setf(&unid, "%s/%u", user_name, id), false,
                  whstr_destroy(&unid));

    CHECKB_RETURN(concat_and_set_path(COLLECTIONS_PATH "/", unid.str), false,
                  whstr_destroy(&unid));

    whstr_destroy(&unid);

    if (whapi.apikey.str) append_query("apikey", whapi.apikey.str);

    format_and_append_purity(purity);

    return true;
}

static bool reset_url(void) {
    CHECK_RETURN(whapi.error_code = curl_url_set(
                     whapi.url, CURLUPART_URL, WALLHAVEN_URL, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    return true;
}

static bool append_query(const char *key, const char *value) {
    whStr query = whstr_create();
    CHECKB_RETURN(whstr_setf(&query, "%s=%s", key, value), false,
                  whstr_destroy(&query));

    CHECK_RETURN(
        whapi.error_code = curl_url_set(whapi.url, CURLUPART_QUERY, query.str,
                                        CURLU_APPENDQUERY | CURLU_URLENCODE),
        false, whstr_destroy(&query);
        whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whstr_destroy(&query);

    return true;
}

static bool concat_and_set_path(const char *p, const char *q) {
    whStr path = whstr_create();

    CHECKB_RETURN(whstr_set(&path, p), false, whstr_destroy(&path));
    CHECKB_RETURN(whstr_append(&path, q), false, whstr_destroy(&path));

    CHECK_RETURN(whapi.error_code = curl_url_set(whapi.url, CURLUPART_PATH,
                                                 path.str, CURLU_URLENCODE),
                 false, whstr_destroy(&path);
                 whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whstr_destroy(&path);

    return true;
}

static void set_response_code_error(unsigned int response_code) {
    switch (response_code) {
        case 200:
            // OK
            break;
        case 401:
            whapi.error_code = WALLHAVEN_UNAUTHORIZED_ACCESS,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            break;
        case 429:
            whapi.error_code = WALLHAVEN_API_CALL_LIMIT_EXCEED,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            break;
        case 404:
            whapi.error_code = WALLHAVEN_NOTHING_IS_HERE,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            break;
        case 400:
            whapi.error_code = WALLHAVEN_BAD_REQUEST,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            break;
        default:
            whapi.error_code = WALLHAVEN_UNKNOWN_ERROR,
            whapi.error_code_type = ERROR_CODE_TYPE_WALLHAVEN;
            break;
    }
}
