#pragma once

#include <stdint.h>
#include "pico/stdlib.h"

// Initialize the TX line (push-pull, idle high) and set bit timing (us per bit).
void tx_init();

// Change bit timing later if needed.
void tx_set_bit_us(uint bit_us);

// Transmit one frame: start(0), 19 bits (LSB-first: 3-bit cmd, 16-bit data), then idle(1).
void tx_frame_bits(uint8_t cmd, int16_t data16);
