#pragma once
#include <stdint.h>
#include <pico/types.h>

#define PIN_TX 0
#define PIN_RX 1

void linker_init(uint bit_us);
void tx_frame_bits(uint8_t cmd, int16_t data16);
void rx_listen_forever(void);
