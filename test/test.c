#include <stddef.h>
#include <whapi/whapi.h>

int main(void) {
    whapi_initialize(true);

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

    whapi_set_apikey(NULL);
    print_apikey();

    whapi_shutdown();
}
