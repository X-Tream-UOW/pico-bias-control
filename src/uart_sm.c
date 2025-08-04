#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "uart.pio.h"
#include "hardware/clocks.h"

#define PIN_RX 1
#define PIN_TX 0
#define UART_BAUD 9600

void uart_tx_program_init(PIO pio, uint sm, uint offset, uint pin, uint baud) {
    pio_sm_config c = uart_tx_program_get_default_config(offset);
    gpio_set_function(PIN_TX, GPIO_FUNC_PIO0);
    pio_gpio_init(pio, pin);

    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_out_pins(&c, pin, 1);
    sm_config_set_out_shift(&c, true, true, 10);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    // sm_config_set_clkdiv(&c, (float)clock_get_hz(clk_sys) / (baud * 10)); // ~10 cycles/bit

    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void uart_rx_program_init(PIO pio, uint sm, uint offset, uint pin, uint baud) {
    pio_sm_config c = uart_rx_program_get_default_config(offset);
    pio_gpio_init(pio, pin);

    sm_config_set_in_pins(&c, pin);
    sm_config_set_in_shift(&c, true, true, 10);
    sm_config_set_clkdiv(&c, (float)clock_get_hz(clk_sys) / (baud * 10));

    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, false);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void uart_tx_program_putc(PIO pio, uint sm, char c) {
    // Construct 10-bit UART frame: [stop_bit=1][8 data bits][start_bit=0]
    uint16_t frame = (1u << 9) | (c << 1);  // stop | data | start
    pio_sm_put_blocking(pio, sm, frame);
}

char uart_rx_program_getc(PIO pio, uint sm) {
    uint16_t raw = pio_sm_get_blocking(pio, sm) & 0x3FF;  // Mask 10 bits
    return (char)((raw >> 1) & 0xFF);  // Extract bits 1–8 (data), skip start/stop
}

void setup_uart_sm() {

    PIO pio = pio0;
    uint sm_tx = 0;
    uint sm_rx = 1;

    gpio_disable_pulls(PIN_TX);
    gpio_disable_pulls(PIN_RX); 

    uint offset_tx = pio_add_program(pio, &uart_tx_program);
    uart_tx_program_init(pio, sm_tx, offset_tx, PIN_TX, UART_BAUD);

    uint offset_rx = pio_add_program(pio, &uart_rx_program);
    uart_rx_program_init(pio, sm_rx, offset_rx, PIN_RX, UART_BAUD);

    const char *msg = "Hello Pi from RP2040!\n";
    for (const char *p = msg; *p; ++p) {
        uart_tx_program_putc(pio, sm_tx, *p);
    }

    while (1) {
        if (!pio_sm_is_rx_fifo_empty(pio, sm_rx)) {
            uint8_t ch = uart_rx_program_getc(pio, sm_rx);
            uart_tx_program_putc(pio, sm_tx, ch); 
        }
    }
}

