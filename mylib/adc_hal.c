#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include <stdio.h>

#define NUM_SAMPLES 1000
#define SAMPLE_FREQ_HZ 50000  // 50kHz = 20us pomiędzy próbkami

uint16_t analogValues[NUM_SAMPLES];
volatile bool dma_done = false;
int dma_chan;

// DMA zakończone — przerwanie
void dma_handler() {
    dma_hw->ints0 = 1u << dma_chan;
    dma_done = true;
    printf("DMA done\n");
}

// Inicjalizacja ADC, DMA i PWM
void adc_dma_pwm_init() {
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    // Konfiguracja FIFO ADC
    adc_fifo_setup(
        true,  // Enable FIFO
        true,  // DREQ enable
        1,     // DREQ at 1 sample
        false,
        false
    );

    // Konfiguracja DMA
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_ADC);

    dma_channel_configure(
        dma_chan,
        &cfg,
        analogValues,       // destination
        &adc_hw->fifo,      // source
        NUM_SAMPLES,
        false               // don't start yet
    );

    // Przerwanie DMA po zakończeniu
    dma_channel_set_irq0_enabled(dma_chan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    // PWM jako zegar próbkowania
    gpio_set_function(22, GPIO_FUNC_PWM);  // dowolny pin PWM, np. GPIO22
    uint slice = pwm_gpio_to_slice_num(22);

    // Oblicz częstotliwość PWM (system clock / divider / wrap)
    pwm_config pwm_cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&pwm_cfg, 1.0f);  // dzielnik = 1
    pwm_config_set_wrap(&pwm_cfg, clock_get_hz(clk_sys) / SAMPLE_FREQ_HZ);

    pwm_init(slice, &pwm_cfg, false);
    pwm_set_gpio_level(22, 1);  // 1-taktowy impuls
}

// Rozpocznij akwizycję: DMA + ADC + PWM
void adc_dma_pwm_start() {
    dma_done = false;
    dma_channel_start(dma_chan);
    adc_run(true);
    pwm_set_enabled(pwm_gpio_to_slice_num(22), true);
}

// Zatrzymaj po zakończeniu
void adc_dma_pwm_stop() {
    pwm_set_enabled(pwm_gpio_to_slice_num(22), false);
    adc_run(false);
    adc_fifo_drain();
}
