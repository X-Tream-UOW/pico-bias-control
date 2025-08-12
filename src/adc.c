#include "adc.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Start conversion: R/C falling edge ≥25 ns then back high.
static inline void start_conversion(void) {
    gpio_put(PIN_ADC_RC, 0);
    sleep_us(1);        // safely above tWL
    gpio_put(PIN_ADC_RC, 1);
}

// STATUS=LOW means data valid and latched.
static inline bool ready(void) {
    return gpio_get(PIN_ADC_STAT) == 0;
}

static inline void wait_ready(void) {
    while (!ready()) {
        tight_loop_contents();
    }
}

// Read 12-bit bus (LSB on PIN_ADC_BASE)
static inline uint16_t read_bus_raw(void) {
    uint16_t v = 0;
    for (int i = 0; i < ADC_BITS; ++i) {
        v |= (gpio_get(PIN_ADC_BASE + i) & 1u) << i; // Using gpio_get instead of sio_hw because no speed requirements
    }
    return v;
}

// Convert raw 12-bit two's complement code (±2048) to volts in ±5.0 V range.
static inline double code_to_volts_bipolar(uint16_t code) {
    int16_t signed_code = (int16_t)(code << 4) >> 4; // sign-extend from 12 bits
    return ((double)signed_code / 2048.0) * 5.0;
}


void adc_init(void) {
    // Data bus inputs
    for (int i = 0; i < ADC_BITS; ++i) {
        gpio_init(PIN_ADC_BASE + i);
        gpio_set_dir(PIN_ADC_BASE + i, false);
        gpio_disable_pulls(PIN_ADC_BASE + i);
    }

    // STAT input
    gpio_init(PIN_ADC_STAT);
    gpio_set_dir(PIN_ADC_STAT, false);
    gpio_disable_pulls(PIN_ADC_STAT);

    // R/C output, idle HIGH
    gpio_init(PIN_ADC_RC);
    gpio_set_dir(PIN_ADC_RC, true);
    gpio_put(PIN_ADC_RC, 1);
}

double adc_read_volts(void) {
    start_conversion();
    wait_ready();
    uint16_t code = read_bus_raw();
    return code_to_volts_bipolar(code);
}
