#include "pico/stdlib.h"
#include "hardware/uart.h"

#define PIN_ADC_BASE 2
#define HV_ADC_COUNT 12
#define PIN_ADC_STAT 14
#define PIN_ADC_RC 15
#define PIN_ADC_CS 16
#define PIN_ADC_CLK 17
#define PIN_ADC_SDI 18
#define PIN_EN 19
#define PIN_EN_CLK 20
#define PIN_SEL 22
#define PIN_LED_0 25
#define PIN_SEL_CLK 26
#define PIN_LED_1 27
#define PIN_LED_2 28

#define UART_ID uart0
#define BAUD_RATE 115200

int main() {
    gpio_init(PIN_LED_0);
    gpio_set_dir(PIN_LED_0, GPIO_OUT);
    gpio_put(PIN_LED_0, 1);

    setup_default_uart();

    while (true) {
        tight_loop_contents();
    }

    return 0;
}