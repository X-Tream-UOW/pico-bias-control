#include "hv_control.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static inline void t_setup(void) { sleep_us(1); }
static inline void t_hold(void)  { sleep_us(1); }

// Rising-edge clock pulse; idle low.
static inline void pulse_clk(uint clk_pin) {
    gpio_put(clk_pin, 1);
    t_hold();
    gpio_put(clk_pin, 0);
}

void hv_init(void) {
    gpio_init(PIN_EN);
    gpio_set_dir(PIN_EN, true);
    gpio_put(PIN_EN, 0);

    gpio_init(PIN_SEL);
    gpio_set_dir(PIN_SEL, true);
    gpio_put(PIN_SEL, 0);

    gpio_init(PIN_EN_CLK);
    gpio_set_dir(PIN_EN_CLK, true);
    gpio_put(PIN_EN_CLK, 0);

    gpio_init(PIN_SEL_CLK);
    gpio_set_dir(PIN_SEL_CLK, true);
    gpio_put(PIN_SEL_CLK, 0);
}

void hv_set_enabled(bool enable) {
    gpio_put(PIN_EN, enable ? 1 : 0);
    t_setup();
    pulse_clk(PIN_EN_CLK);
}

void hv_set_polarity(hv_polarity_t polarity) {
    gpio_put(PIN_SEL, polarity);
    t_setup();
    pulse_clk(PIN_SEL_CLK);
}
