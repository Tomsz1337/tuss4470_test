#include "pulse_gen.h"
#include "pulse_gen.pio.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"

int8_t pulse_gen_program_init(PIO pio, uint pin, uint freqHz) {
    uint offset = pio_add_program(pio, &pulse_gen_program);
    int sm = pio_claim_unused_sm(pio, true);
    if (sm < 0) return -1;

    pio_sm_config c = pulse_gen_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    gpio_set_function(pin, GPIO_FUNC_PIO0);

    uint sys_clk = clock_get_hz(clk_sys);
    const uint instr_per_cycle = 10; // 2x set + 2x set y + 2x loop + 4x nop
    uint divider = (sys_clk << 8) / (freqHz * instr_per_cycle);
    sm_config_set_clkdiv_int_frac(&c, divider >> 8, divider & 0xFF);

    pio_sm_init(pio, sm, offset, &c);
    return sm;
}

void pulse_gen_start(PIO pio, int8_t sm, uint pulses) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);
    pio_sm_put_blocking(pio, sm, pulses);
    pio_sm_set_enabled(pio, sm, true);
}
