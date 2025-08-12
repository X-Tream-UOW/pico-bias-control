#pragma once
#include <stdbool.h>

#include "hv_control.h"

// Initialise DAC, ADC, and HV control modules
void bias_init(void);

// Set the desired bias voltage (in volts at DAC input, before HV gain)
void bias_set_voltage(double volts);

// Set bias polarity (positive or negative)
void bias_set_polarity(hv_polarity_t polarity);

// Turn bias ON (enable HV module)
void bias_on(void);

// Turn bias OFF (disable HV module)
void bias_off(void);

// Get measured bias voltage from ADC (after HV gain)
double bias_get_voltage(void);

// Get bias enable status (true = ON)
bool bias_get_status(void);
