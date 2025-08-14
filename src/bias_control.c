#include "bias_control.h"
#include "dac.h"
#include "adc.h"
#include "hv_control.h"
#include "pico/stdlib.h"
#include "led_control.h"


static bool bias_enabled = false;

void bias_init(void) {
    dac_init(DAC_REF_INTERNAL, 2.500, 12);
    adc_init();
    hv_init();
    bias_enabled = false;
}

void bias_set_voltage(double volts) {
    dac_write_input_voltage(volts);
    sleep_ms(1);
    dac_update_output();
}

void bias_set_polarity(hv_polarity_t polarity) {
    hv_set_polarity(polarity);
}

void bias_on(void) {
    hv_set_enabled(true);
    bias_enabled = true;
    led_hv_on();
}

void bias_off(void) {
    hv_set_enabled(false);
    bias_enabled = false;
    led_hv_off();
}

double bias_get_voltage(void) {
    return adc_read_volts();
}

bool bias_get_status(void) {
    return bias_enabled;
}
