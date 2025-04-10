#pragma once

#include "hardware/pio.h"
#include "pico/stdlib.h"

void pulse_gen_program_init(PIO pio, uint sm, uint offset, uint pin);
void pulse_gen_start(PIO pio, uint sm, uint num_pulses);
