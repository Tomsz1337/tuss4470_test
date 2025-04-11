#pragma once

#ifdef RP2040

#include "hardware/pio.h"

void pulse_gen_program_init(PIO pio, uint32_t sm, uint32_t offset, uint32_t pin, uint32_t freqHz);
void pulse_gen_start(PIO pio, uint32_t sm, uint32_t num_pulses);
#else

#endif