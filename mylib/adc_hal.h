#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdint.h>
#include <stdbool.h>

// Liczba próbek do pobrania (musi się zgadzać z tablicą w pliku .c)
#define NUM_SAMPLES 1000

// Częstotliwość próbkowania (w Hz), np. 50000 = 20µs odstęp
#define SAMPLE_FREQ_HZ 50000

// Bufor na dane ADC (może być zadeklarowany tu jako extern)
extern uint16_t analogValues[NUM_SAMPLES];

// Flaga informująca o zakończeniu transferu DMA
extern volatile bool dma_done;

/**
 * @brief Inicjalizuje ADC, DMA i PWM trigger.
 * 
 * Należy wywołać raz przed użyciem adc_dma_pwm_start().
 * Konfiguruje ADC z FIFO, DMA do bufora, PWM do taktowania próbkowania.
 */
void adc_dma_pwm_init(void);

/**
 * @brief Rozpoczyna akwizycję danych: ADC + DMA + PWM trigger.
 * 
 * Wypełnia bufor `analogValues`. Zakończenie jest sygnalizowane przez `dma_done == true`.
 */
void adc_dma_pwm_start(void);

/**
 * @brief Zatrzymuje ADC, PWM i opróżnia FIFO.
 * 
 * Należy wywołać po zakończeniu akwizycji.
 */
void adc_dma_pwm_stop(void);

#endif // ADC_DMA_H
