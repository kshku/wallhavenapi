#include <stdio.h>
#include <whapi/whapi.h>

void test_parsed(const char *user, const char *apikey);

void test_raw(const char *user, const char *apikey);

void test_raw_to_file(const char *user, const char *apikey);

int main(void) {
    const char *apikey = "YOUR_API_KEY";
    const char *user = "YOUR_USER_NAME";

    test_parsed(user, apikey);
    test_raw(user, apikey);
    test_raw_to_file(user, apikey);
}
