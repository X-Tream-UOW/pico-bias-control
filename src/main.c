// #define PIN_LED_0 25
// #define PIN_LED_1 27
// #define PIN_LED_2 28

#include "pico/stdlib.h"
#include "dac.h"
#include "adc.h"
#include "hv_control.h"
#include <stdlib.h>  
#include <stdio.h>

static inline int32_t volts_to_mV(double v) {
    // Convert double volts (±5.0 V range from adc_read_volts) to mV safely
    int32_t mv = (int32_t)(v * 1000.0);
    return mv;
}

int main(void) {
    stdio_init_all();

    dac_init(DAC_REF_INTERNAL, 2.500, 12);
    adc_init();
    hv_init();

    const double dac_target_volts = 0.1;  // gain is ~ x40
    dac_write_input_voltage(dac_target_volts);
    dac_update_output();

    hv_set_polarity(HV_NEGATIVE);
    sleep_ms(10);
    hv_set_enabled(true);

    sleep_ms(10);

    double v_adc = adc_read_volts(); 
    int32_t mv   = volts_to_mV(v_adc);

    printf("ADC: %+ld.%03ld V (target DAC set %.3f V)\n",
           mv / 1000, labs(mv % 1000),
           dac_target_volts);

    while (1) {
        sleep_ms(1000);
        v_adc = adc_read_volts();
        mv    = volts_to_mV(v_adc);
        printf("ADC: %+ld.%03ld V\n", mv / 1000, labs(mv % 1000));
    }
}

