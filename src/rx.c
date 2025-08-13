#include "pico/stdlib.h"
#include "frame_parser.h"
#include <stdint.h>
#include <stdio.h>

#define PIN_RX 1

// Reads a frame: start bit (low), then 19 bits of data, then high idle
// bit_us = duration of one bit in microseconds
uint32_t read_frame(uint32_t bit_us) {
    uint32_t data = 0;

    // Wait for line to be high
    while (!gpio_get(PIN_RX)) 
        tight_loop_contents();

    // Wait for line to go low (start bit)
    while (gpio_get(PIN_RX))
        tight_loop_contents();

    // Wait half bit period to sample in the middle
    sleep_us(bit_us / 2);

    // Verify still low (start bit check)
    if (gpio_get(PIN_RX)) {
        // False start
        return 0xFFFFFFFF;
    }

    // Read 19 data bits, LSB first
    for (int i = 0; i < 19; i++) {
        sleep_us(bit_us);
        uint32_t bit = gpio_get(PIN_RX);
        data |= (bit << i);
    }

    sleep_us(bit_us);

    return data;
}


void rx_init() {
    gpio_init(PIN_RX);
    gpio_set_dir(PIN_RX, false);
    gpio_pull_up(PIN_RX);
}   

void read_rx(uint bit_us) {

    while (true) {
        uint32_t frame = read_frame(bit_us);
        if (frame == 0xFFFFFFFF) {
            // printf("Framing error\n");
            continue;
        }

        // Decode: 3-bit command (bits 0..2), 16-bit signed data (bits 3..18), LSB-first
        uint8_t  cmd  = (uint8_t)(frame & 0x7u);
        int16_t  data = (int16_t)(frame >> 3);

        // for (int i = 0; i <= 18; i+"+) printf("%d", (frame >> i) & 1);
        // printf("cmd=%u data=%d\n", cmd, data);

        process_frame(cmd, data);
    }
}
