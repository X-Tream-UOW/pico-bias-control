#include "pico/stdlib.h"

#define PIN_LED_0 25
#define PIN_LED_1 27
#define PIN_LED_2 28

void led_init(int pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}

void led_on(int pin){
    gpio_put(pin, 1);
}

void led_off(int pin){
    gpio_put(pin, 0);
}

void leds_init() {
    led_init(PIN_LED_0);
    led_init(PIN_LED_1);
    led_init(PIN_LED_2);
}

void led_alive_on() {
    led_on(PIN_LED_0);
}

void led_hv_on() {
    led_on(PIN_LED_1);
}

void led_hv_off() {
    led_off(PIN_LED_1);
}

void led_rx_on() {
    led_on(PIN_LED_2);
}

void led_rx_off() {
    led_off(PIN_LED_2);
}