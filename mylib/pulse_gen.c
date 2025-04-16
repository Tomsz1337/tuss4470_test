#include "pulse_gen.h"
#include "hardware/pio.h"
#include <stdio.h>

#include "pulse_gen.pio.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

int8_t pulse_gen_program_init(PIO pio, uint32_t pin, uint32_t freqHz) 
{
    uint32_t offset = pio_add_program(pio, &pulse_gen_program);
    pio_sm_config c = pulse_gen_program_get_default_config(offset);
    int8_t sm = 0;

    gpio_set_function(pin, GPIO_FUNC_PIO0);
    sm_config_set_set_pins(&c, pin, 1);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    const uint32_t sys_clk = 125000000;
    const uint32_t instr_per_pulse = 10; // bo 2x set + 4x nop + jmp

    uint32_t divider = (sys_clk << 8) / (freqHz * instr_per_pulse);
    uint32_t div_int = divider >> 8;
    uint32_t div_frac = divider & 0xFF;

    sm_config_set_clkdiv_int_frac(&c, div_int, div_frac);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_clear_fifos(pio, sm);
    
    return sm;
}


void pulse_gen_start(PIO pio, int8_t sm, uint32_t num_pulses)
{
    
    pio_sm_set_enabled(pio, sm, false); // upewnij się, że maszyna jest wyłączona
    pio_sm_clear_fifos(pio, sm);        // wyczyść FIFO
    pio_sm_restart(pio, sm);            // restart

    pio_sm_put_blocking(pio, sm, num_pulses);
    pio_sm_set_enabled(pio, sm, true);

   
    sleep_us(500);  // dopasuj czas do liczby impulsów i taktowania
    pio_sm_set_enabled(pio, sm, false);
    
}
