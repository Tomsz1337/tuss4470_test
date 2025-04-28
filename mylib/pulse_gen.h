#pragma once
#include "hardware/pio.h"

int8_t pulse_gen_program_init(PIO pio, uint32_t pin, uint32_t freqHz);
void pulse_gen_start(PIO pio, int8_t sm, uint32_t num_pulses);
