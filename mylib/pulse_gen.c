#include "pulse_gen.h"
#include "pulse_gen.pio.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"

void pulse_gen_program_init(PIO_settings *sPIO,uint32_t freqHz, uint pin) 
{
    sPIO->pio = pio0;
    sPIO->sm = pio_claim_unused_sm(sPIO->pio, true);
    uint offset = pio_add_program(sPIO->pio, &pulse_gen_program);
    float div = (float)clock_get_hz(clk_sys) / freqHz / 4;
    pio_sm_config c = pulse_gen_program_get_default_config(offset);
    pio_gpio_init(sPIO->pio, pin);
    sm_config_set_set_pins(&c, pin, 1);
    pio_sm_set_consecutive_pindirs(sPIO->pio, sPIO->sm, pin, 1, true);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(sPIO->pio, sPIO->sm, offset, &c);
}

void pulse_gen_start(PIO pio, uint sm, uint32_t nPulses) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);
    pio_sm_put_blocking(pio, sm, nPulses - 1);
    pio_sm_set_enabled(pio, sm, true);
}
