#include "pico/stdlib.h"
#include "dac.h"
#include "adc.h"
#include "hv_control.h"
#include "rx.h"
#include "tx.h"
#include "led_control.h"
#include <stdlib.h>  
#include <stdio.h>

#define BIT_US 1000  // 1Khz communication line

int main(void) {
    leds_init();
    led_alive_on();

    dac_init(DAC_REF_INTERNAL, 2.500, 12);
    adc_init();
    hv_init();

    rx_init();
    tx_init();
    tx_set_bit_us(BIT_US);

    read_rx(BIT_US);
}

