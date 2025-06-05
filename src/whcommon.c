#include "whcommon.h"

// Global state
whAPI whapi = {0};

size_t write_to_whstr(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    whStr *r = (whStr *)userp;

    CHECKB_RETURN(whstr_appendn(r, buffer, realsize), 0);

    return realsize;
}

size_t write_to_file(void *buffer, size_t size, size_t nmemb, void *userp) {
    FILE *fp = (FILE *)userp;
    return fwrite(buffer, size, nmemb, fp);
}

bool reset_url(void) {
    CHECK_RETURN(whapi.error_code = curl_url_set(
                     whapi.url, CURLUPART_URL, WALLHAVEN_URL, CURLU_URLENCODE),
                 false, whapi.error_code_type = ERROR_CODE_TYPE_URL);

    return true;
}

bool append_query(const char *key, const char *value) {
    whStr query = whstr_create();

    CHECKB_RETURN(whstr_set(&query, key), false, whstr_destroy(&query));
    CHECKB_RETURN(whstr_appendn(&query, "=", 1), false, whstr_destroy(&query));
    CHECKB_RETURN(whstr_append(&query, value), false, whstr_destroy(&query));

#ifdef WH_DEBUG
    printf("Appending query: %s\n", query.str);
#endif

    CHECK_RETURN(
        whapi.error_code = curl_url_set(whapi.url, CURLUPART_QUERY, query.str,
                                        CURLU_APPENDQUERY | CURLU_URLENCODE),
        false, whstr_destroy(&query);
        whapi.error_code_type = ERROR_CODE_TYPE_URL);

    whstr_destroy(&query);

    return true;
}

bool concat_and_set_path(const char *p, const char *q) {
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

    whapi.retry = whapi.response_code_handler(response_code);

    return true;
}

// TODO: Validating
static bool format_and_append_q(Query *q) {
    whStr query = whstr_create();

    if (q->id) {
        if ((q->like || q->tags || q->type != IMAGE_TYPE_NONE
             || q->user_name)) {
            whstr_destroy(&query);
            return false;
        }

        CHECKB_RETURN(whstr_appendn(&query, "id:", 3), false,
                      whstr_destroy(&query));
        CHECKB_RETURN(whstr_append(&query, q->id), false,
                      whstr_destroy(&query));
    } else {
        if (q->tags)
            CHECKB_RETURN(whstr_append(&query, q->tags), false,
                          whstr_destroy(&query));

        if (q->user_name) {
            CHECKB_RETURN(whstr_appendn(&query, " @", 2), false,
                          whstr_destroy(&query));
            CHECKB_RETURN(whstr_append(&query, q->user_name), false,
                          whstr_destroy(&query));
        }

        if (q->type != IMAGE_TYPE_NONE) {
            CHECKB_RETURN(whstr_appendn(&query, " type:", 6), false,
                          whstr_destroy(&query));
            CHECKB_RETURN(
                whstr_append(&query, q->type == IMAGE_TYPE_PNG ? "png" : "jpg"),
                false, whstr_destroy(&query));
        }

        if (q->like) {
            CHECKB_RETURN(whstr_appendn(&query, " like:", 6), false,
                          whstr_destroy(&query));
            CHECKB_RETURN(whstr_append(&query, q->like), false,
                          whstr_destroy(&query));
        }
    }

    CHECKB_RETURN(append_query("q", query.str), false, whstr_destroy(&query));

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
    for (int i = 2; i > -1; --i, categories >>= 1)
        cat[i] = (categories & 1) + '0';

    CHECKB_RETURN(append_query("categories", cat), false);

    return true;
}

static bool format_and_append_purity(unsigned int purity) {
    if (purity == 0) return true;

    if (purity & PURITY_NSFW) CHECKP_RETURN(whapi.apikey.str, false);

    char pur[4] = {0};
    for (int i = 2; i > -1; --i, purity >>= 1) pur[i] = (purity & 1) + '0';

    CHECKB_RETURN(append_query("purity", pur), false);

    return true;
}

static bool format_and_append_sorting(Sorting sorting) {
    if (sorting == SORTING_NONE) return true;

    char *sor[] = {
        [SORTING_DATE_ADDED] = "date_added", [SORTING_RELEVANCE] = "relevance",
        [SORTING_RANDOM] = "random",         [SORTING_VIEWS] = "views",
        [SORTING_FAVORITES] = "favorites",   [SORTING_TOPLIST] = "toplist"};

    CHECKB_RETURN(append_query("sorting", sor[sorting]), false);

    return true;
}

static bool format_and_append_order(Order order) {
    if (order == ORDER_NONE) return true;

    char *ord[] = {[ORDER_ASCENDING] = "asc", [ORDER_DESCENDING] = "desc"};

    CHECKB_RETURN(append_query("order", ord[order]), false);

    return true;
}

static bool format_and_append_toprange(TopRange toprange, Sorting sorting) {
    if (toprange == TOPRANGE_NONE) return true;

    if (sorting != SORTING_TOPLIST) return false;

    char *top[] = {
        [TOPRANGE_1D] = "1d", [TOPRANGE_3D] = "3d", [TOPRANGE_1W] = "1w",
        [TOPRANGE_1M] = "1M", [TOPRANGE_3M] = "3M", [TOPRANGE_6M] = "6M",
        [TOPRANGE_1Y] = "1y"};

    CHECKB_RETURN(append_query("topRange", top[toprange]), false);

    return true;
}

static bool format_and_append_atleast_resolution(Resolution res) {
    if (res.width == 0 && res.height == 0) return true;

    char buffer[20];

    snprintf(buffer, 20, "%ux%u", res.width, res.height);

    CHECKB_RETURN(append_query("atleast", buffer), false);

    return true;
}

static bool format_and_append_exact_resolution(Resolution *res,
                                               unsigned int n) {
    if (n == 0) return true;

    whStr resolutions = whstr_create();
    char buffer[20] = {0};
    size_t len = snprintf(buffer, 20, "%ux%u", res[0].width, res[0].height);
    CHECKB_RETURN(whstr_setn(&resolutions, buffer, len), false,
                  whstr_destroy(&resolutions));

    for (unsigned int i = 1; i < n; ++i) {
        len = snprintf(buffer, 20, ",%ux%u", res[i].width, res[i].height);
        CHECKB_RETURN(whstr_appendn(&resolutions, buffer, len), false,
                      whstr_destroy(&resolutions));
    }

    CHECKB_RETURN(append_query("resolutions", resolutions.str), false,
                  whstr_destroy(&resolutions));

    whstr_destroy(&resolutions);

    return true;
}

static bool format_and_append_ratios(Ratio *ratios, unsigned int n) {
    if (n == 0) return true;

    whStr ra = whstr_create();
    char buffer[20] = {0};
    size_t len =
        snprintf(buffer, 20, "%ux%u", ratios[0].width, ratios[0].height);
    CHECKB_RETURN(whstr_setn(&ra, buffer, len), false, whstr_destroy(&ra));

    for (unsigned int i = 1; i < n; ++i) {
        len = snprintf(buffer, 20, ",%ux%u", ratios[i].width, ratios[i].height);
        CHECKB_RETURN(whstr_appendn(&ra, buffer, len), false,
                      whstr_destroy(&ra));
    }

    CHECKB_RETURN(append_query("ratios", ra.str), false, whstr_destroy(&ra));

    whstr_destroy(&ra);

    return true;
}

static bool format_and_append_colors(Color *colors, unsigned int n) {
    if (n == 0) return true;

    whStr col = whstr_create();
    char buffer[10] = {0};
    size_t len = snprintf(buffer, 10, "%06x", colors[0]);
    CHECKB_RETURN(whstr_setn(&col, buffer, len), false, whstr_destroy(&col));

    for (unsigned int i = 1; i < n; ++i) {
        len = snprintf(buffer, 10, ",%06x", colors[i]);
        CHECKB_RETURN(whstr_appendn(&col, buffer, len), false,
                      whstr_destroy(&col));
    }

    CHECKB_RETURN(append_query("colors", col.str), false, whstr_destroy(&col));

    whstr_destroy(&col);

    return true;
}

static bool format_and_append_page(unsigned int page) {
    if (page == 0) return true;

    char buffer[20];
    snprintf(buffer, 20, "%u", page);

    CHECKB_RETURN(append_query("page", buffer), false);

    return true;
}

static bool format_and_append_seed(const char seed[7]) {
    if (seed[0] == 0) return true;

    for (int i = 0; seed[i]; ++i) {
        if ((seed[i] < 'a' || seed[i] > 'z') && (seed[i] < 'A' || seed[i] > 'Z')
            && (seed[i] < '0' || seed[i] > '9'))
            return false;
    }

    CHECKB_RETURN(append_query("seed", seed), false);

    return true;
}

bool format_and_append_search_parameters(SearchParameters *params) {
    CHECKB_RETURN(format_and_append_q(&params->q), false);

    CHECKB_RETURN(format_and_append_categories(params->categories), false);

    CHECKB_RETURN(format_and_append_purity(params->purity), false);

    CHECKB_RETURN(format_and_append_sorting(params->sorting), false);

    CHECKB_RETURN(format_and_append_order(params->order), false);

    CHECKB_RETURN(format_and_append_toprange(params->toprange, params->sorting),
                  false);

    CHECKB_RETURN(
        format_and_append_atleast_resolution(params->atleast_resolution),
        false);
    CHECKB_RETURN(
        format_and_append_exact_resolution(params->exact_resolutions,
                                           params->exact_resolution_count),
        false);

    CHECKB_RETURN(format_and_append_ratios(params->ratios, params->ratio_count),
                  false);

    CHECKB_RETURN(format_and_append_colors(params->colors, params->color_count),
                  false);

    CHECKB_RETURN(format_and_append_page(params->page), false);

    CHECKB_RETURN(format_and_append_seed(params->seed), false);

    return true;
}
