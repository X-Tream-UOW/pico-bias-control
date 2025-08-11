

// #define PIN_ADC_BASE 2
// #define HV_ADC_COUNT 12
// #define PIN_ADC_STAT 14
// #define PIN_ADC_RC 15
// #define PIN_EN 19
// #define PIN_EN_CLK 20
// #define PIN_SEL 22
// #define PIN_LED_0 25
// #define PIN_SEL_CLK 26
// #define PIN_LED_1 27
// #define PIN_LED_2 28


#include "pico/stdlib.h"
#include "dac.h"  

int main(void) {
    stdio_init_all();

    // Init DAC: using internal reference, 2.500 V, 12-bit resolution
    dac_init(DAC_REF_INTERNAL, 2.500, 12);

    // Example: write 1.25 V to input register
    dac_write_input_voltage(1.25);

    // Now latch it to the output
    dac_update_output();

    // Optional: wait, then power down
    // sleep_ms(2000);
    // dac_power_down();

    while (1) {
        // Main loop – nothing else to do
        tight_loop_contents();
    }
}
