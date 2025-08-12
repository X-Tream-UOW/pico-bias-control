#pragma once
#include <stdint.h>
#include <stdbool.h>

#define PIN_ADC_BASE  2
#define ADC_BITS      12
#define PIN_ADC_STAT  14
#define PIN_ADC_RC    15

// Initialise GPIOs and idle states for the ADS774 in 12-bit bipolar mode.
void adc_init(void);

// Trigger a conversion, wait until ready, read the signed 12-bit code,
// and return the corresponding voltage in ±5.0 V range.
double adc_read_volts(void);
