#include "frame_parser.h"
#include "bias_control.h"
#include <stdio.h>
#include "tx.h"

static uint8_t s_pol = 0;

// This functions takes the command and the data from the RX module and execute the associated functions
void process_frame(uint8_t cmd, int16_t data) {

    printf("Command: %u, Data: %d\n", cmd, data);

    switch (cmd) {
    case CMD_SET_VOLTAGE:
        bias_set_voltage((double)data / 1000.0);
        break;

    case CMD_SET_POLARITY:
        s_pol = (uint8_t)(data & 1);
        bias_set_polarity(s_pol ? HV_NEGATIVE : HV_POSITIVE);
        break;

    case CMD_ON:
        bias_on();
        break;

    case CMD_OFF:
        bias_off();
        break;

    case CMD_GET_STATUS: {
        uint8_t flags = (bias_get_status() ? 1u : 0u) | (uint8_t)(s_pol << 1);
        tx_frame_bits(CMD_GET_STATUS, (int16_t)flags);
    } break;

    case CMD_GET_BIAS: {
        double v_adc = bias_get_voltage();
        double v_hv  = v_adc * 40.0;  // The gain can be tweaked here
        int16_t hv_dV = (int16_t)(v_hv * 10.0); // Converting to decivolts to get an int16
        tx_frame_bits(CMD_GET_BIAS, hv_dV);
    } break;

    default:
        break;
    }
}
