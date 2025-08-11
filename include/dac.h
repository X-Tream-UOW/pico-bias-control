#pragma once
#include <stdint.h>

#define PIN_DAC_CS   16
#define PIN_DAC_CLK  17
#define PIN_DAC_SDI  18

typedef enum {
    DAC_REF_INTERNAL, 
    DAC_REF_VCC      
} dac_ref_mode_t;

// Initialize GPIOs and (optionally) select reference mode.
// resolution_bits (here 12)
// vref_volts must match the actual reference used (e.g., 2.500, 4.096, or VCC).
void dac_init(dac_ref_mode_t ref_mode, double vref_volts, int resolution_bits);

void dac_select_reference(dac_ref_mode_t ref_mode);

// ---- High-level API ----

// Command 0: write input register only (no output change)
void dac_write_input_code(uint16_t code);
void dac_write_input_voltage(double volts);

// Command 1: update output from input register (latch). Also powers up if powered down.
void dac_update_output(void);

// Command 4: power down the DAC output.
void dac_power_down(void);

// Convert voltage to code with rounding/saturation using initialized resolution/Vref.
uint16_t dac_voltage_to_code(double volts);

