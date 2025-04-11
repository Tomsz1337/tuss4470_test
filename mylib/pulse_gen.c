#include "pulse_gen.h"
#include "pulse_gen.pio.h"

void pulse_gen_program_init(PIO pio, uint32_t sm, uint32_t offset, uint32_t pin, uint32_t freqHz) 
{
    pio_sm_config c = pulse_gen_program_get_default_config(offset);

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
}

void pulse_gen_start(PIO pio, uint32_t sm, uint32_t num_pulses)
{
    pio_sm_put_blocking(pio, sm, num_pulses);
    pio_sm_set_enabled(pio, sm, true);

    while (!pio_sm_is_tx_fifo_empty(pio, sm))
    {
        tight_loop_contents();
    }
    pio_sm_set_enabled(pio, sm, false);
}