#pragma once
#include <stdbool.h>
#include <stdint.h>

#define PIN_EN       19
#define PIN_EN_CLK   20
#define PIN_SEL      22
#define PIN_SEL_CLK  26

typedef enum {
    HV_POSITIVE = 0,  // SEL low
    HV_NEGATIVE = 1   // SEL high
} hv_polarity_t;

// Configure GPIO directions and idle states (CLK low).
void hv_init(void);

// Latch enable state into the HV path (true = enable, false = disable).
void hv_set_enabled(bool enable);

// Latch bias polarity (HV_POSITIVE or HV_NEGATIVE).
void hv_set_polarity(hv_polarity_t polarity);
