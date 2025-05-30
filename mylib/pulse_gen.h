#pragma once

#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PIO_settings
{
    PIO pio;
    uint sm;
}PIO_settings;

/// Initiazlie PIO program
/// \param pio        PIO instance (pio0/pio1)
/// \param pin        Output pin
/// \param sm         State machine number(0-3)
void pulse_gen_program_init(PIO_settings *sPIO,uint32_t freqHz, uint pin);

/// Starts pulse generation
/// \param pio        PIO instance (pio0/pio1)
/// \param sm         State machine number(0-3)
/// \param pulses     Number of pulses
void pulse_gen_start(PIO pio, uint sm, uint32_t nPulses);

#ifdef __cplusplus
}
#endif
