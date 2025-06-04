#include <stdio.h>
#include <whapi/whapi.h>

int main(void) {
    if (!whapi_initialize(true)) {
        whapi_shutdown();
        return -1;
    }

    print_apikey();

    whapi_set_apikey("XYZ");
    print_apikey();

    whapi_set_apikey("Really "
                     "loooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
                     "oooooong strings "
                     "ksladfjalsdjfl;asjdflasjdfl;ajsdlfjas;dfjal;sdjfal;"
                     "sdjfasjhkdfkasjhdfj");
    print_apikey();

    whapi_set_apikey("XYZ");
    print_apikey();

    whStr temp = whstr_create();
    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    printf("Response: %s\n", temp.str);

    whstr_clear(&temp);

    whapi_set_apikey(NULL);
    print_apikey();

    whapi_get_wallpaper_info_raw("gwjq3d", &temp);
    printf("Response: %s\n", temp.str);

    whstr_destroy(&temp);

    whapi_shutdown();

    whStr str = whstr_create();

    whstr_append(&str, "something is ");
    printf("%s\n", str.str);

    whstr_set(&str, "Something is ");
    printf("%s\n", str.str);

    whstr_append(&str, "better than ");
    printf("%s\n", str.str);

    whstr_append(&str, "nothing!");
    printf("%s\n", str.str);

    whstr_destroy(&str);
}
