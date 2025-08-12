// #define PIN_LED_0 25
// #define PIN_LED_1 27
// #define PIN_LED_2 28


#include "pico/stdlib.h"
#include "hv_control.h"

int main(void) {
    stdio_init_all();

    sleep_ms(1000);

    hv_init();
    hv_set_polarity(HV_NEGATIVE);
    hv_set_enabled(true);

    sleep_ms(1000);

    hv_set_enabled(false);
    sleep_ms(1000);
    hv_set_polarity(HV_POSITIVE);
    hv_set_enabled(true);

    sleep_ms(1000);

    hv_set_enabled(false);

    // while (1) {
    //     tight_loop_contents();
    // }
}

