// #define PIN_LED_0 25
// #define PIN_LED_1 27
// #define PIN_LED_2 28

#include "pico/stdlib.h"
#include "dac.h"
#include "adc.h"
#include "hv_control.h"
#include "rx.h"
#include <stdlib.h>  
#include <stdio.h>

int main(void) {
    stdio_init_all();

    dac_init(DAC_REF_INTERNAL, 2.500, 12);
    adc_init();
    hv_init();
    sleep_ms(5);
    hv_set_enabled(true);

    read_rx();

    while (true) {
        tight_loop_contents();
    }

}

