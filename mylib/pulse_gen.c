#include "pulse_gen.h"
#include "pulse_gen.pio.h"

void pulse_gen_program_init(PIO pio, uint sm, uint offset, uint pin) 
{
    pio_sm_config c = pulse_gen_program_get_default_config(offset);

    sm_config_set_set_pins(&c, pin, 1);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    sm_config_set_clkdiv(&c, 1.0f); // 1 instrukcja na cykl

    pio_sm_init(pio, sm, offset, &c);
}

void pulse_gen_start(PIO pio, uint sm, uint num_pulses)
{
    pio_sm_put_blocking(pio, sm, num_pulses);
    pio_sm_set_enabled(pio, sm, true);

    // Czekaj aż fifo się opróżni (czyli SM skończy)
    while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
        tight_loop_contents();
    }

    pio_sm_set_enabled(pio, sm, false);
}