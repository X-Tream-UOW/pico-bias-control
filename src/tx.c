#include "tx.h"

#define PIN_TX 0

static uint tx_bit_us = 0;

void tx_init() {
    gpio_init(PIN_TX);
    gpio_set_dir(PIN_TX, true);   // output
    gpio_put(PIN_TX, 1);          // idle high
}

void tx_set_bit_us(uint bit_us) {
    tx_bit_us = bit_us;
}

void tx_frame_bits(uint8_t cmd, int16_t data16) {
    // Pack 3-bit cmd (LSBs) and 16-bit data
    uint32_t word19 = ((uint32_t)(uint16_t)data16 << 3) | (uint32_t)(cmd & 0x7);

    // Ensure idle high before start
    gpio_put(PIN_TX, 1);
    sleep_us(tx_bit_us);

    // Start bit (low)
    gpio_put(PIN_TX, 0);
    sleep_us(tx_bit_us);

    // 19 data bits, LSB-first
    for (int i = 0; i < 19; i++) {
        uint32_t bit = (word19 >> i) & 1u;
        gpio_put(PIN_TX, bit);
        sleep_us(tx_bit_us);
    }

    // Stop / idle high
    gpio_put(PIN_TX, 1);
    sleep_us(tx_bit_us);
}
