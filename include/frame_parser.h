#pragma once
#include <stdint.h>

#define CMD_SET_VOLTAGE  0x01  // DATA=int16 mV
#define CMD_SET_POLARITY 0x02  // DATA bit0: 0=POS, 1=NEG
#define CMD_ON           0x03  // DATA ignored
#define CMD_OFF          0x04  // DATA ignored
#define CMD_GET_STATUS   0x05  // reply DATA: flags (bit0=en, bit1=pol)
#define CMD_GET_BIAS     0x06  // reply DATA: hv in deci-volts (int16)

void process_frame(uint8_t cmd, int16_t data16);
