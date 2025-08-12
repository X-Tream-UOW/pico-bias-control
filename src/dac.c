#include "dac.h"
#include "pico/stdlib.h"
#include <math.h>

static double   s_vref_volts = 2.500;
static int      s_resolution = 12;
static uint16_t s_fullscale  = 0x0FFF;

enum {
    CMD_WRITE_INPUT      = 0x0,  // write input reg
    CMD_UPDATE_DAC       = 0x1,  // update (and power up)
    CMD_WRITE_AND_UPDATE = 0x3,  // write + update (and power up)
    CMD_POWER_DOWN       = 0x4,
    CMD_SEL_INT_REF      = 0x6,
    CMD_SEL_SUPPLY_REF   = 0x7
};


static inline void t_delay(void) { sleep_us(1); }  // 1 cycle = 2us => 500khz

static inline void gpio_if_init(void) {
    gpio_init(PIN_DAC_CS);
    gpio_init(PIN_DAC_CLK);
    gpio_init(PIN_DAC_SDI);
    gpio_set_dir(PIN_DAC_CS,  true);
    gpio_set_dir(PIN_DAC_CLK, true);
    gpio_set_dir(PIN_DAC_SDI, true);
    gpio_put(PIN_DAC_CS, 1);  
    gpio_put(PIN_DAC_CLK, 0); 
    gpio_put(PIN_DAC_SDI, 0);
}

static inline void clock_bit(uint8_t bit) {
    gpio_put(PIN_DAC_SDI, bit);
    t_delay();
    gpio_put(PIN_DAC_CLK, 1);
    t_delay();
    gpio_put(PIN_DAC_CLK, 0);
}

static void shift24(uint32_t w) {
    for (int i = 23; i >= 0; --i) {
        clock_bit((w >> i) & 1u);
    }
}

static inline uint16_t data_align(uint16_t codeN) {
    // Left-align N-bit code into the 16-bit data field (pad LSBs with zeros).
    return (uint16_t)(codeN << (16 - s_resolution));
}

static uint32_t build_frame(uint8_t cmd, uint16_t codeN) {
    uint32_t w = 0;
    w |= ((uint32_t)(cmd & 0xF) << 20);  // command bits 23..20
    // bits 19..16 are don't-care = 0
    w |= data_align(codeN);              // bits 15..0
    return w;
}

static inline uint16_t mask_for_res(void) {
    switch (s_resolution) {
        case 12: return 0x0FFF;
        case 10: return 0x03FF;
        case 8:  return 0x00FF;
        default: return 0;
    }
}

static inline void xfer(uint32_t frame24) {
    gpio_put(PIN_DAC_CS, 0);
    t_delay();
    shift24(frame24);
    t_delay();
    gpio_put(PIN_DAC_CS, 1);
    t_delay();
}

// Public API
void dac_init(dac_ref_mode_t ref_mode, double vref_volts, int resolution_bits) {
    gpio_if_init();
   
    s_resolution = resolution_bits;
    s_fullscale  = (1u << s_resolution) - 1u;

    if (vref_volts <= 0.0) vref_volts = 2.500;
    s_vref_volts = vref_volts;

    dac_select_reference(ref_mode);
}

void dac_select_reference(dac_ref_mode_t ref_mode) {
    uint8_t cmd = (ref_mode == DAC_REF_VCC) ? CMD_SEL_SUPPLY_REF : CMD_SEL_INT_REF;
    xfer(build_frame(cmd, 0));
}

uint16_t dac_voltage_to_code(double volts) {
    if (volts < 0.0) volts = 0.0;
    if (volts > s_vref_volts) volts = s_vref_volts;
    double fs = (double)s_fullscale;
    uint32_t code = (uint32_t)llround((volts / s_vref_volts) * fs);
    if (code > s_fullscale) code = s_fullscale;
    return (uint16_t)code;
}

void dac_write_input_code(uint16_t code) {
    uint16_t mask = mask_for_res();
    uint16_t c = code & mask;
    xfer(build_frame(CMD_WRITE_INPUT, c));
}

// Command 0: write input register only
void dac_write_input_voltage(double volts) {
    dac_write_input_code(dac_voltage_to_code(volts));
}

// Command 1: update output (also powers up)
void dac_update_output(void) {
    xfer(build_frame(CMD_UPDATE_DAC, 0));
}


// Command 4: power down
void dac_power_down(void) {
    xfer(build_frame(CMD_POWER_DOWN, 0));
}
