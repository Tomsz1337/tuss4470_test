#include "pulse_gen.h"
#include "hardware/pio.h"


#ifdef RP2040

#include "pulse_gen.pio.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

static inline bool pio_sm_is_enabled(PIO pio, uint sm) 
{
    return (pio->ctrl & (1u << sm)) != 0;
}

int8_t find_free_sm(PIO pio)
{
    for (int sm = 0; sm < 4; ++sm) 
    {
        if (!pio_sm_is_enabled(pio, sm)) 
        {
            return sm;
        }
    }
    return -1;
}

int8_t pulse_gen_program_init(PIO pio, uint32_t pin, uint32_t freqHz) 
{
    uint32_t offset = pio_add_program(pio, &pulse_gen_program);
    pio_sm_config c = pulse_gen_program_get_default_config(offset);
    int8_t sm = find_free_sm(pio);

    sm_config_set_set_pins(&c, pin, 1);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    
    const uint32_t sys_clk = 125000000;
    const uint32_t instr_per_pulse = 4;
 
    // Q16.8 format
    uint32_t divider = (sys_clk << 8) / (freqHz * instr_per_pulse);
    uint32_t div_int = divider >> 8;
    uint32_t div_frac = divider & 0xFF;
 
    sm_config_set_clkdiv_int_frac(&c, div_int, div_frac);
    pio_sm_init(pio, sm, offset, &c);

    return sm;
}

void pulse_gen_start(PIO pio, int8_t sm, uint32_t num_pulses)
{
    pio_sm_put_blocking(pio, sm, num_pulses);
    pio_sm_set_enabled(pio, sm, true);

    while (!pio_sm_is_tx_fifo_empty(pio, sm))
    {
        tight_loop_contents();
    }
    pio_sm_set_enabled(pio, sm, false);
}
#else

#endif