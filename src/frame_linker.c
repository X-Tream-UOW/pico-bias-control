#include "frame_linker.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static uint s_bit_us;

static inline void tx_hi(void){ gpio_put(PIN_TX, 1); }
static inline void tx_lo(void){ gpio_put(PIN_TX, 0); }
static inline int  rx_rd(void){ return gpio_get(PIN_RX); }

static inline uint8_t parity_even(uint8_t cmd, uint16_t data16){
    uint32_t x = (uint32_t)cmd | ((uint32_t)data16 << 3);
    x ^= x >> 16; x ^= x >> 8; x ^= x >> 4; x ^= x >> 2; x ^= x >> 1;
    return (uint8_t)(x & 1u);
}

void linker_init(uint bit_us) {
    s_bit_us = bit_us;

    gpio_init(PIN_TX);
    gpio_set_dir(PIN_TX, true);
    tx_hi(); // idle high

    gpio_init(PIN_RX);
    gpio_set_dir(PIN_RX, false);
    gpio_pull_up(PIN_RX); // idle high
}

void tx_frame_bits(uint8_t cmd, int16_t data16) {
    tx_lo(); sleep_us(2*s_bit_us);              // START
    for (int i=0;i<3;i++){                      // CMD (3 bits LSB-first)
        (cmd>>i)&1 ? tx_hi() : tx_lo();
        sleep_us(s_bit_us);
    }
    uint16_t d = (uint16_t)data16;              // DATA (16 bits LSB-first)
    for (int i=0;i<16;i++){
        (d>>i)&1 ? tx_hi() : tx_lo();
        sleep_us(s_bit_us);
    }
    uint8_t p = parity_even(cmd, d);           // PARITY (even)
    p ? tx_hi() : tx_lo();
    sleep_us(s_bit_us);
    tx_hi(); sleep_us(3*s_bit_us);              // STOP
}

static int wait_start(void){
    while (rx_rd()!=0) { tight_loop_contents(); }
    sleep_us(s_bit_us);
    if (rx_rd()!=0) return -1;
    sleep_us(s_bit_us/2);
    while (rx_rd()==0) { tight_loop_contents(); }
    return 0;
}

static int rx_frame_bits(uint8_t *cmd, int16_t *data16) {
    if (wait_start()<0) return -1;
    sleep_us(s_bit_us/2);

    uint8_t c=0;
    for (int i=0;i<3;i++){ c |= (rx_rd()&1)<<i; sleep_us(s_bit_us); }

    uint16_t d=0;
    for (int i=0;i<16;i++){ d |= (rx_rd()&1)<<i; sleep_us(s_bit_us); }

    uint8_t p = rx_rd()&1;
    sleep_us(s_bit_us);

    if (p != parity_even(c, d)) return -2;

    *cmd = c;
    *data16 = (int16_t)d;
    return 0;
}

void rx_listen_forever(void){
    while (true) {
        uint8_t cmd; int16_t data;
        if (rx_frame_bits(&cmd, &data)==0) {
            process_frame(cmd, data);
        }
    }
}